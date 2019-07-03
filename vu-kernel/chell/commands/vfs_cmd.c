#include "fs/vfs.h"
#include "memory/memutil.h"

char *pwd = (char *)"hda";

void vfs_cmd_main() { vfs_ls(); }

void vfs_cmd_ls() { vfs_ls_dir("/hda/"); }

void vfs_cmd_search() { vfs_touch("/hda/newfile"); };

char *vfs_cmd_pwd() { return pwd; }

int vfs_cmd_cd(char *dir) {
  int res = vfs_cd(dir);
  if (res == 1)
    memcpy((uint8_t *) pwd, (uint8_t *)dir, 3);
  return res;
}
