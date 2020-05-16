#ifndef FS_H
#define FS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>


#define DISK "rsc/disk"
#define BLOC_SIZE 1024
#define INODE_FIELD_SIZE 32


typedef enum filetype {
	REGULAR_FILE, DIRECTORY, SYMBOLIC_LINK, FIFO, SOCKET, DEVICE
} filetype;

struct inode {

	unsigned char id;

	filetype type;
	mode_t permissions;

	unsigned char *user_name;
	unsigned char *group_name;

	struct tm *created_at;
	struct tm *updated_at;

	unsigned int *bloc_ids;
};

struct inode create_inode(filetype type, mode_t perms, unsigned char *user, unsigned char *group);
void print_inode(struct inode *i);
int write_inode(struct inode *i);

#endif

