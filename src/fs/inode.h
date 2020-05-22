#ifndef INODE_H
#define INODE_H

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define ROOT_ID (1)
#define USERNAME_COUNT (15)
#define GROUPNAME_COUNT (15)
#define BLOC_IDS_COUNT (10)
#define DELETED (0)
#define TODO_PRINT printf("TODO line %d\n", __LINE__)

/* TODO discard typedef, see Torvald's recommandations */
typedef enum filetype {
	REGULAR_FILE, DIRECTORY, SYMBOLIC_LINK, FIFO, SOCKET, DEVICE
} filetype;

/**
 * Stores metadata of blocs
 */
struct inode {
	unsigned char id;

	/* const */
	filetype type;
	mode_t permissions;

	char user_name[USERNAME_COUNT];
	char group_name[GROUPNAME_COUNT];

	/* const */
	struct tm *created_at;
	struct tm *updated_at;

	unsigned int bloc_ids[BLOC_IDS_COUNT];
	int bloc_count;
};

int contains(struct inode *i, unsigned int bloc_id);
int inode_equals(struct inode i1, struct inode i2);
struct inode new_inode(filetype type, mode_t perms, const char *user, const char *group);
void init_id_generator();
void print_inode(struct inode *i);
struct inode empty_inode();

#endif

