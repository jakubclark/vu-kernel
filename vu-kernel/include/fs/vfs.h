#ifndef VFS_H
#define VFS_H

#include "std/types.h"

#define MAX_DEVICES 26

typedef struct {
    char name[32];
    uint32_t flags;
    uint32_t len;
    uint32_t eof;
    uint32_t dev;
    uint32_t current_cluster;
    uint32_t type;
} file;

typedef struct {
    void (*mount) ();
    void (*read) (file *f, char *str);
    void (*write) (file *f, char *str);
    void (*close) (file *f);
    file (*open) (char *name);
    void (*ls) (char *dir);
    file (*cd) (char *dir);
    int (*touch) (char *name);
    int (*delete) (char *name);
} filesystem;

#define FS_FILE     0
#define FS_DIR      1
#define FS_NULL     2

extern void vfs_init();
extern void vfs_ls();
extern void vfs_ls_dir(char *dir);
extern int vfs_cd(char *name);
extern int vfs_touch(char *name);
extern int vfs_delete(char *name);
extern file *vfs_file_open(char *name, uint8_t *mode);
extern file *vfs_file_open_user(char *name, uint8_t *mode);
extern void vfs_file_read(file *f, char *str);
extern void vfs_file_write(file *f, char *str);
extern void vfs_file_close(file *f);
extern void vfs_file_close_user(file *f);
extern int vfs_get_dev(char *name);
extern void vfs_mount(char *name);
extern void vfs_unmount(char *name);

#endif

