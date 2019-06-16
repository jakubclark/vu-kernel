/*
 *  Copyright 2016 Davide Pianca
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef DEVICE_H
#define DEVICE_H

#include "fs/vfs.h"

typedef struct fat_mount_info {
  /* FS type
   * 0 = FAT12
   * 1 = FAT16
   * 2 = FAT32
   * 3 = EXFAT
   */
  int type;
  uint32_t n_sectors;
  uint32_t fat_offset;
  uint32_t n_root_entries;
  uint32_t root_offset;
  uint32_t root_size;
  uint32_t fat_size;
  uint32_t fat_entry_size;
  uint32_t first_data_sector;
  uint32_t data_sectors;
} fat_mount_info_t;

typedef struct device {
  int id;
  int type; // 0 floppy, 1 ata hdd
  char mount[4];
  char *(*read)(int lba);
  int (*write)(int lba);
  filesystem fs;
  fat_mount_info_t minfo;
} device_t;

extern void device_register(device_t *dev);
extern device_t *get_dev_by_name(char *name);
extern device_t *get_dev_by_id(int id);
extern int get_dev_id_by_name(char *name);

#endif
