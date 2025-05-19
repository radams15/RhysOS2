#include "fs.h"

#include <type.h>
#include <string.h>

#define MAX_OPEN_FILES 64
#define MAX_MOUNTS 4

FsNode_t* open_files[MAX_OPEN_FILES];
FsMount_t mounts[MAX_MOUNTS] = {NULL};
FsNode_t* fs_root;

FsNode_t* fh_get_node(int fh) {
    return open_files[fh];
}

int fs_mount(const char* name, FsNode_t* parent, FsNode_t* child) {
    for (int i = 0; i < MAX_MOUNTS; i++) {
        if (mounts[i].parent == NULL && mounts[i].child == NULL) {
            strcpy(mounts[i].name, (char*)name);
            mounts[i].parent = parent;
            mounts[i].child = child;
            return 0;
        }
    }

    return 1;
}

int write(int fh, unsigned char* buffer, unsigned int size) {
    return fs_write(fh_get_node(fh), fh_get_node(fh)->offset, size, buffer);
}

int read(int fh, unsigned char* buffer, unsigned int size) {
    return fs_read(fh_get_node(fh), fh_get_node(fh)->offset, size, buffer);
}

void seek(int fh, unsigned int location) {
    fh_get_node(fh)->offset = location;
}

int open(char* name, FileMode_t mode) {
    int i;
    FsNode_t* handle;

    handle = get_dir((char*)name);

    if (handle == NULL) {
        if (mode & O_CREAT) {
            /* handle = create_file(name); */
            return -1;
        } else {
            return -1;
        }

        if ((int)handle == -1) {
            /* print_string("Could not find directory: '"); */
            /* print_string(name); */
            /* print_string("'\n"); */
            return -1;
        }
    }

    for (i = 0; i < MAX_OPEN_FILES; i++) {
        if (open_files[i] == NULL) {
            open_files[i] = handle;

            seek(i, 0);  // Go to start of file

            if (mode & O_RDONLY) {
                open_files[i]->write = NULL;
            }

            if (mode & O_WRONLY) {
                open_files[i]->read = NULL;
            }

            if (mode & O_APPEND) {
                // TODO: Go to end of file
            }

            return i;
        }
    }

    return -1;
}

void close(int fh) {
    open_files[fh] = NULL;
}

FsNode_t* get_dir(char* name) {
    FsNode_t* fsnode = fs_root;
    char* tok;

    char buf[256];
    memcpy((char*)&buf, name, strlen(name));
    buf[strlen(name)] = 0;

    tok = strtok(buf, "/");

    while (tok != NULL && fsnode != NULL) {
        for (int i = 0; i < MAX_MOUNTS; i++) {
            if (fsnode == mounts[i].parent && mounts[i].parent != NULL &&
                mounts[i].child != NULL && strcmp(tok, mounts[i].name) == 0) {
                fsnode = mounts[i].child;
                goto next;
            }
        }

        fsnode = fs_finddir(fsnode, tok);
    next:
        tok = strtok(NULL, "/");
    }
    return fsnode;
}

unsigned int fs_read(FsNode_t* node,
                     unsigned int offset,
                     unsigned int size,
                     unsigned char* buffer) {
    if (node->read != 0) {
        return node->read(node, offset, size, buffer);
    }

    return 0;
}

unsigned int fs_write(FsNode_t* node,
                      unsigned int offset,
                      unsigned int size,
                      unsigned char* buffer) {
    if (node->write != 0) {
        return node->write(node, offset, size, buffer);
    }

    return 0;
}

unsigned int fs_close(FsNode_t* node) {
    if (node->close != 0) {
        return node->close(node);
    }

    return 0;
}

DirEnt_t* fs_readdir(FsNode_t* node, unsigned int index) {
    if (node->readdir != 0 && (node->flags & 0x07) == FS_DIRECTORY) {
        return node->readdir(node, index);
    }

    return 0;
}

FsNode_t* fs_finddir(FsNode_t* node, char* name) {
    if (node->finddir != 0 && (node->flags & 0x07) == FS_DIRECTORY) {
        return node->finddir(node, name);
    }

    return 0;
}

/* int list_directory(char* dir_name, FsNode_t* buf, int max, int ds) { */
/*     int i = 0; */
/*     int count = 0; */
/*     DirEnt_t* node = NULL; */
/*     FsNode_t* fsnode; */
/*     FsNode_t* root = get_dir(dir_name); */
/*  */
/*     if (root == NULL) { */
/*         print_string("Cannot find directory!\n"); */
/*         return 0; */
/*     } */
/*  */
/*     while ((node = fs_readdir(root, i)) != NULL && count <= max) { */
/*         fsnode = fs_finddir(root, node->name); */
/*         if (fsnode != NULL) { */
/*             if (buf != NULL) { */
/*                 buf++; */
/*                 seg_copy((char*)fsnode, (char*)buf, sizeof(FsNode_t), */
/*                          KERNEL_DATA_SEGMENT, ds); */
/*                 seg_copy(fsnode->name, buf->name, strlen(fsnode->name), */
/*                          KERNEL_DATA_SEGMENT, ds); */
/*             } */
/*             count++; */
/*         } */
/*  */
/*         i++; */
/*     } */
/*  */
/*     return count; */
/* } */
