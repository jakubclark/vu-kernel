#ifndef __VFS_CMD_H
#define __VFS_CMD_H

extern void vfs_cmd_main();
extern void vfs_cmd_ls();
extern void vfs_cmd_search();
extern char* vfs_cmd_pwd();
extern int vfs_cmd_cd(char* dir);

#endif
