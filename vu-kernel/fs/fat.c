#include "device.h"
#include "fs/fat.h"
#include "fs/mbr.h"
#include "io/scrn.h"
#include "memory/memutil.h"
#include "memory/physmem.h"
#include "std/string.h"
#include "std/types.h"

int offset;
uint8_t FAT[SECTOR_SIZE * 2];

void fat_mount(device_t *dev) {
  bootsector_t *bs = kmalloc(sizeof(bootsector_t));

  dev->read(0, (uint8_t *)bs);
  if ((bs->ignore[0] != 0xEB) || (bs->ignore[2] != 0x90))
    return;

  mbr_t *mbr = (mbr_t *)bs;
  for (int i = 0; i < 4; i++) {
    if (mbr->partition_table[i].sys_id != FAT32_SYSTEM_ID) {
      continue;
    } else {
      uint32_t lba = mbr->partition_table[i].lba_start;
      dev->read(lba, (uint8_t *)bs);
      break;
    }
    kfree((uint32_t)bs);
    return;
  }

  dev->minfo.n_sectors =
      (bs->bpb.n_sectors == 0) ? bs->bpb.long_sectors : bs->bpb.n_sectors;
  dev->minfo.fat_offset = bs->bpb.reserved_sectors;
  dev->minfo.fat_size = (bs->bpb.fat_sectors == 0) ? bs->bpb_ext.fat_sectors
                                                   : bs->bpb.fat_sectors;
  dev->minfo.fat_entry_size = 8;
  dev->minfo.n_root_entries = bs->bpb.n_dir_entries;
  dev->minfo.root_offset = (bs->bpb.n_fats * dev->minfo.fat_size) + 1;
  dev->minfo.root_size =
      ((bs->bpb.n_dir_entries * 32) + (bs->bpb.sector_bytes - 1)) /
      bs->bpb.sector_bytes;
  dev->minfo.first_data_sector = dev->minfo.fat_offset +
                                 (bs->bpb.n_fats * dev->minfo.fat_size) +
                                 dev->minfo.root_size;
  dev->minfo.data_sectors =
      bs->bpb.n_sectors -
      (bs->bpb.reserved_sectors + (bs->bpb.n_fats * dev->minfo.fat_size) +
       dev->minfo.root_size);

  uint32_t total_clusters = dev->minfo.data_sectors / bs->bpb.cluster_sectors;
  if (total_clusters < 4085)
    dev->minfo.type = FAT12;
  else if (total_clusters < 65525)
    dev->minfo.type = FAT16;
  else if (total_clusters < 268435445)
    dev->minfo.type = FAT32;
  else
    dev->minfo.type = EXFAT;
  kfree((uint32_t)bs);
}

void to_dos_file_name(char *name, char *str) {
  if ((!name) || (!str))
    return;

  memset_a(str, ' ', NAME_LEN);
  int i;
  for (i = 0; i < strlen(name) && i < NAME_LEN; i++) {
    if ((name[i] == '.') || (i == 8))
      break;
    str[i] = to_upper(name[i]);
  }

  if (name[i] == '.') {
    for (int j = 0; j < 3; j++) {
      i++;
      if (name[i])
        str[8 + j] = to_upper(name[i]);
    }
  }
  str[NAME_LEN] = 0;
}

void to_normal_file_name(char *name, char *str) {
  int j = 0, flag = 1;

  if ((!name) || (!str))
    return;

  memset_a(str, ' ', NAME_LEN);
  for (int i = 0; i < strlen(name) && i < NAME_LEN; i++) {
    if (name[i] != ' ') {
      str[j] = to_lower(name[i]);
      j++;
    } else if ((flag == 1) && (name[9] != ' ')) {
      flag = 0;
      str[j] = '.';
      j++;
    }
  }
  str[j] = 0;
}

void print_dir(directory_t *dir) {
  printf("%s %s %d %d %d\n", dir->filename, dir->extension, dir->attrs,
         dir->first_cluster, dir->file_size);
}

uint32_t get_phys_sector(file *f) { return 32 + f->current_cluster - 1; }

directory_t *fat_get_dir(file *f) {
  char *dos_file_name = kmalloc(NAME_LEN);
  to_dos_file_name(f->name, dos_file_name);
  device_t *dev = get_dev_by_id(f->dev);

  directory_t *dir = kmalloc(SECTOR_SIZE);

  for (int i = 0; i < 14; i++) {
    dev->read(i, (uint8_t *)dir);
    for (int j = 0; j < 16; j++, dir++) {
      if (strncmp(dos_file_name, (char *)dir->filename, NAME_LEN) == 0) {
        offset = i;
        kfree((uint32_t)dos_file_name);
        return dir;
      }
    }
  }
  kfree((uint32_t)dos_file_name);
  return NULL;
}


void fat_read(file *f, char *buf) {
  if (!f)
    return;

  device_t *dev = get_dev_by_id(f->dev);
  uint8_t *sector = kmalloc(SECTOR_SIZE);
  dev->read(get_phys_sector(f), sector);
  memcpy_a(buf, sector, SECTOR_SIZE);

  uint32_t fat_offset;
  switch (dev->minfo.type) {
  case FAT12:
    fat_offset = f->current_cluster + (f->current_cluster / 2);
    break;
  case FAT16:
    fat_offset = f->current_cluster * 2;
    break;
  case FAT32:
    fat_offset = f->current_cluster * 4;
    break;
  default:
    return;
  }
  uint32_t fat_sector = dev->minfo.fat_offset + (fat_offset / SECTOR_SIZE);
  uint32_t entry_offset = fat_offset % SECTOR_SIZE;

  dev->read(fat_sector, sector);
  memcpy_a(FAT, sector, SECTOR_SIZE);

  dev->read(fat_sector + 1, sector);
  memcpy_a(FAT + SECTOR_SIZE, sector, SECTOR_SIZE);

  if (dev->minfo.type == FAT12) {
    uint16_t next_cluster = *(uint16_t *)&FAT[entry_offset];
    if (f->current_cluster & 0x0001)
      next_cluster >>= 4;
    else
      next_cluster &= 0x0FFF;

    if ((next_cluster >= 0xFF8) || (next_cluster == 0)) {
      f->eof = 1;
      return;
    }
    f->current_cluster = next_cluster;
  } else if (dev->minfo.type == FAT16) {
    uint16_t next_cluster = *(uint16_t *)&FAT[entry_offset];
    if ((next_cluster >= 0xFFF8) || (next_cluster == 0)) {
      f->eof = 1;
      return;
    }
    f->current_cluster = next_cluster;
  } else if (dev->minfo.type == FAT32) {
    uint32_t next_cluster = *(uint32_t *)&FAT[entry_offset] & 0x0FFFFFFF;
    if ((next_cluster >= 0x0FFFFFF8) || (next_cluster == 0)) {
      f->eof = 1;
      return;
    }
    f->current_cluster = next_cluster;
  }
}

file fat_directory(char *dir_name, int devid) {
  file f;
  strcpy(f.name, dir_name);
  f.dev = devid;
  f.eof = 0;

  directory_t *dir = fat_get_dir(&f);
  if (dir) {
    f.current_cluster = dir->first_cluster;
    f.len = dir->file_size;
    if (dir->attrs & 0x10)
      f.type = FS_DIR;
    else
      f.type = FS_FILE;
  } else {
    f.type = FS_NULL;
  }
  return f;
}

file fat_open_subdir(file directory, char *name) {
  file f;
  strcpy(f.name, name);
  char *dos_file_name = kmalloc(NAME_LEN);
  to_dos_file_name(name, dos_file_name);
  char *buf = kmalloc(SECTOR_SIZE);

  while (!directory.eof) {
    fat_read(&directory, buf);
    directory_t *dir = (directory_t *)buf;
    for (int i = 0; i < 16; i++) {
      if (strncmp(dos_file_name, (char *)dir->filename, NAME_LEN) == 0) {
        f.current_cluster = dir->first_cluster;
        f.len = dir->file_size;
        f.eof = 0;
        f.dev = directory.dev;
        if (dir->attrs & 0x10)
          f.type = FS_DIR;
        else
          f.type = FS_FILE;
        kfree((uint32_t)buf);
        kfree((uint32_t)dos_file_name);
        return f;
      }
      dir++;
    }
  }
  kfree((uint32_t)buf);
  kfree((uint32_t)dos_file_name);
  f.type = FS_NULL;
  return f;
}

file fat_open(char *name) { return fat_search(name); }

file fat_cd(char *dir) { return fat_search(dir); }

file fat_search(char *name) {
  file cur_dir;
  int root = 1;

  cur_dir.dev = get_dev_id_by_name(name);
  name += 3;
  while (name++) {
    char pathname[16];
    int i;
    for (i = 0; i < 16; i++) {
      if ((name[i] == '/') || (name[i] == '\0'))
        break;
      pathname[i] = name[i];
    }
    pathname[i] = 0;
    if (root) {
      cur_dir = fat_directory(pathname, cur_dir.dev);
      root = 0;
    } else {
      cur_dir = fat_open_subdir(cur_dir, pathname);
    }
    name = strchr(name, '/');
  }
  return cur_dir;
}

void fat_ls(char *dir) {
  char *normal_name = kmalloc(NAME_LEN);

  device_t *dev = get_dev_by_name(dir);

  directory_t *direc = kmalloc(sizeof(directory_t));

  for (int i = 8; i < 14; i++) {

    dev->read(i, (uint8_t *)direc);

    for (int j = 0; j < 16; j++, direc++) {
      if (((char *)direc->filename)[0] == 0)
        continue;
      to_normal_file_name((char *)direc->filename, normal_name);
      printf("file_size=%d\tdate_created=%d\tname=%s | first_cluster=%d\n",
              direc->date_created, direc->file_size, normal_name,
              direc->first_cluster);
    }
  }
  kfree((uint32_t)normal_name);
}

void fat_init(filesystem *fs_fat) {
  fs_fat->mount = &fat_mount;
  fs_fat->read = &fat_read;
  fs_fat->open = &fat_open;
  fs_fat->ls = &fat_ls;
  fs_fat->cd = &fat_cd;
}
