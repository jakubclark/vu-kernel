#ifndef __VFS_H
#define __VFS_H

#include "std/types.h"

#define MAX_DEVICES 10

#define FS_FILE     0
#define FS_DIR      1
#define FS_NULL     2

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

/* Initialize the Virtual FileSystem */
extern void vfs_init();
/* List all the currently mounted devices */
extern void vfs_ls();
/* List the contents of the directory `dir` */
extern void vfs_ls_dir(char *dir);
/* Change to the directory referenced to by `name` */
extern int vfs_cd(char *name);
/* Create a new file at the path `name` */
extern int vfs_touch(char *name);
/* Delete the file at the path `name` (if it exists) */
extern int vfs_delete(char *name);
/* Open the file at the path `name` */
extern file *vfs_file_open(char *name, uint8_t *mode);
/* Open the file at the path `name` */
/* Read the contents of the file `f` and store the result in `str` */
extern void vfs_file_read(file *f, char *str);
/* Write the contents of `str` to the file `f` */
extern void vfs_file_write(file *f, char *str);
/* Close the file `f` */
extern void vfs_file_close(file *f);
/* Get the device with the name `name` */
extern int vfs_get_dev(char *name);
/* Mount the device called `name` */
extern void vfs_mount(char *name);
/* Unmount the deviced called `name` */
extern void vfs_unmount(char *name);

#endif
