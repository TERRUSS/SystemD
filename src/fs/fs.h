#ifndef FS_H
#define FS_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


#define DISK "rsc/disk"

#define BLOC_SIZE 1024

extern const int INODE_FLAG;
extern const int BLOC_FLAG;


typedef enum filetype {
	REGULAR_FILE, DIRECTORY, SYMBOLIC_LINK, FIFO, SOCKET, DEVICE
} filetype;

struct inode {

	unsigned char id;

	filetype type;
	mode_t permissions;

	char user_name[15];
	char group_name[15];

	struct tm *created_at;
	struct tm *updated_at;

	unsigned int bloc_ids[10];
};

struct bloc {
	unsigned int id;
	unsigned char filename[15];
	unsigned char content[1024];
};

struct inode create_inode(filetype type, mode_t perms, const char *user, const char *group);
void print_inode(struct inode *i);
int write_inode(struct inode *i);
int print_disk();

#endif

