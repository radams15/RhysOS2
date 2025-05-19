#ifndef FS_H
#define FS_H

#define FILE_NAME_MAX 32

enum FsFlag {
    FS_FILE = 0x01,
    FS_DIRECTORY,
    FS_CHARDEVICE,
    FS_BLOCKDEVICE,
    FS_PIPE,
    FS_SYMLINK,
    FS_MOUNTPOINT
};

typedef enum FileMode {
    O_CREAT  = 0b00000001,
    O_APPEND = 0b00000010,
    O_RDONLY = 0b00000100,
    O_WRONLY = 0b00001000,
} FileMode_t;

struct FsNode;

typedef unsigned int (*ReadFunc)(struct FsNode*,
                                 unsigned int,
                                 unsigned int,
                                 unsigned char*);
typedef unsigned int (*WriteFunc)(struct FsNode*,
                                  unsigned int,
                                  unsigned int,
                                  unsigned char*);
typedef struct FsNode* (*CreateFunc)(struct FsNode* parent, const char* name);
typedef unsigned int (*CloseFunc)(struct FsNode*);
typedef struct DirEnt* (*ReaddirFunc)(struct FsNode*, unsigned int);
typedef struct FsNode* (*FinddirFunc)(struct FsNode*, char* name);

typedef struct DirEnt {
    char name[FILE_NAME_MAX];
    unsigned int inode;
} DirEnt_t;

typedef struct FsNode {
    char name[FILE_NAME_MAX];
    unsigned int flags;  // node type, etc
    unsigned int inode;
    unsigned int start_sector;
    unsigned int length;  // size in bytes.
    unsigned int offset;  // used by seek
    ReadFunc read;
    WriteFunc write;
    CreateFunc create;
    CloseFunc close;
    ReaddirFunc readdir;
    FinddirFunc finddir;
    struct FsNode* ref;  // Pointer to symlink or mount
} FsNode_t;

typedef struct FsMount {
    char name[FILE_NAME_MAX];
    FsNode_t* parent;
    FsNode_t* child;
} FsMount_t;

unsigned int fs_read(FsNode_t* node,
                     unsigned int offset,
                     unsigned int size,
                     unsigned char* buffer);
unsigned int fs_write(FsNode_t* node,
                      unsigned int offset,
                      unsigned int size,
                      unsigned char* buffer);
unsigned int fs_open(FsNode_t* node, unsigned char read, unsigned char write);
unsigned int fs_close(FsNode_t* node);
DirEnt_t* fs_readdir(FsNode_t* node, unsigned int index);
FsNode_t* fs_finddir(FsNode_t* node, char* name);
int fs_mount(const char* name, FsNode_t* parent, FsNode_t* child);

FsNode_t* get_dir(char* name);
int write(int fh, unsigned char* buffer, unsigned int size);
int open(char* name, FileMode_t mode);
void close(int fh);
void seek(int fh, unsigned int location);
int read(int fh, unsigned char* buffer, unsigned int size);
// int list_directory(char* dir_name, FsNode_t* buf, int max, int ds);

extern FsNode_t* fs_root;

#endif
