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
#define FILENAME_COUNT 15
#define USERNAME_COUNT 15
#define GROUPNAME_COUNT 15
#define BLOC_IDS_COUNT 10

/* Globals */

extern const int INODE_FLAG;
extern const int BLOC_FLAG;
extern const mode_t DEFAULT_PERMISSIONS;

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
	int bloc_count;
};

struct bloc {
	unsigned int id;
	char filename[FILENAME_COUNT];
	char content[BLOC_SIZE];
};

/* Prototypes */

int contains(struct inode *i, unsigned int bloc_id);
int print_disk();
int write_bloc(struct bloc *b);
int write_inode(struct inode *i);
struct bloc create_bloc(const char *filename, const char *content);
struct inode create_inode(filetype type, mode_t perms, const char *user, const char *group);
struct inode create_regularfile(char *filename, const char *mode);
struct inode create_root();
unsigned int get_bloc_id(char *filename);
void add_bloc(struct inode *i, struct bloc *b);
void print_bloc(struct bloc *b);
void print_inode(struct inode *i);

#endif

