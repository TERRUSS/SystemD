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

/* Macros */

#define DISK "rsc/disk"

#define BLOC_SIZE 1024
#define USERNAME_COUNT 15
#define GROUPNAME_COUNT 15
#define BLOC_IDS_COUNT 10

/* Globals */

extern const int INODE_FLAG;
extern const int BLOC_FLAG;

/* Enumerations */

typedef enum filetype {
	REGULAR_FILE, DIRECTORY, SYMBOLIC_LINK, FIFO, SOCKET, DEVICE
} filetype;

/* Structures */

/**
 * Stores metadata of blocs
 */
struct inode {
	unsigned char id;

	filetype type;
	mode_t permissions;

	char user_name[USERNAME_COUNT];
	char group_name[GROUPNAME_COUNT];

	struct tm *created_at;
	struct tm *updated_at;

	unsigned int bloc_ids[BLOC_IDS_COUNT];
};

struct bloc {
	unsigned int id;
	char filename[15];
	char content[1024];
};

/* Prototypes */

struct inode create_inode(filetype type, mode_t perms, const char *user, const char *group);
void print_inode(struct inode *i);
int write_inode(struct inode *i);
struct bloc create_bloc(const char *filename, const char *content);
void print_bloc(struct bloc *b);
int write_bloc(struct bloc *b);
int print_disk();

#endif

