#include "devfs.h"

#include <type.h>
#include <string.h>
#include <arch/vga.h>

#define MAX_FILES 16
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static FsNode_t root_node;
static FsNode_t root_nodes[MAX_FILES];
static int num_root_nodes;

static DirEnt_t dirent;


DirEnt_t* devfs_readdir(FsNode_t* node, unsigned int index) {
    if (index >= num_root_nodes + 1) {
        return NULL;
    }

    strcpyz(dirent.name, root_nodes[index - 1].name);
    dirent.name[strlen(root_nodes[index - 1].name)] = 0;
    dirent.inode = root_nodes[index - 1].inode;

    return &dirent;
}

FsNode_t* devfs_finddir(FsNode_t* node, char* name) {
    int i;
    for (i = 0; i < num_root_nodes; i++) {
        if (strcmp(name, root_nodes[i].name) == 0) {
            return &root_nodes[i];
        }
    }

    return NULL;
}


unsigned int stdout_write(FsNode_t* node,
                          unsigned int offset,
                          unsigned int size,
                          unsigned char* buffer) {
    int i;

    for (i = offset; i < size; i++) {
        vga_putc(buffer[i]);
    }

    return size;
}

void devfs_setup() {
    int i = 0;

    strcpyz(root_nodes[i].name, "stdout");
    root_nodes[i].flags = FS_FILE;
    root_nodes[i].inode = i;
    root_nodes[i].length = 1;
    root_nodes[i].offset = 0;
    root_nodes[i].read = 0;
    root_nodes[i].write = stdout_write;
    root_nodes[i].create = 0;
    root_nodes[i].close = 0;
    root_nodes[i].readdir = 0;
    root_nodes[i].finddir = 0;
    root_nodes[i].ref = 0;
    num_root_nodes++;
}


FsNode_t* devfs_init() {
    strcpyz(root_node.name, "dev");
    root_node.flags = FS_DIRECTORY;
    root_node.inode = 0;
    root_node.length = 0;
    root_node.offset = 0;
    root_node.read = 0;
    root_node.write = 0;
    root_node.create = 0;
    root_node.close = 0;
    root_node.readdir = devfs_readdir;
    root_node.finddir = devfs_finddir;
    root_node.ref = 0;

    num_root_nodes = 0;

    devfs_setup();

    return &root_node;
}
