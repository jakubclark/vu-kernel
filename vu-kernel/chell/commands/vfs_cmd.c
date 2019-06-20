#include "fs/vfs.h"

void vfs_cmd_main() { vfs_ls(); }

void vfs_cmd_ls() { vfs_ls_dir("/hda/"); }

void vfs_cmd_search(){
    vfs_touch("/hda/newfile");
};
