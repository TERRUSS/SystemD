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
#define ROOT_ID (1)

#define BLOC_SIZE (512)
#define FILENAME_COUNT (15)
#define USERNAME_COUNT (15)
#define GROUPNAME_COUNT (15)
#define BLOC_IDS_COUNT (10)
#define LAST_BLOC (3)
#define NOT_LAST_BLOC (4)
#define DELETED (0)
#define TODO_PRINT printf("TODO line %d\n", __LINE__)

/* Constants */

extern const int INODE_FLAG;
extern const int BLOC_FLAG;
extern const mode_t DEFAULT_PERMISSIONS;

/* Globals */

extern struct inode g_current_node;
extern struct file g_filetree;

/* Enumerations */

// TODO discard typedef, see Torvald's recommandations
typedef enum filetype {
	REGULAR_FILE, DIRECTORY, SYMBOLIC_LINK, FIFO, SOCKET, DEVICE
} filetype;

/* Structures */

/**
 * Stores metadata of blocs
 */
struct inode {
	/* const */
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

struct bloc {
	/* const */
	unsigned int id;
	char filename[FILENAME_COUNT];
	char content[BLOC_SIZE];
	/* A file's content can be scattered among multiple blocs
	 * Can be set to LAST_BLOC or NOT_LAST_BLOC
	 */
	unsigned int last_bloc;
};

struct file {
	struct inode i;
	struct bloc b;
	struct file *files;
	int files_count;
};

/* Prototypes */

int clean_disk();
int contains(struct inode *i, unsigned int bloc_id);
int print_disk();
int strncut(char ***str_array, char *str, size_t n);
int update_bloc_content(unsigned int bloc_id, const char *new_content);
int update_inode(struct inode *new_inode);
int write_bloc(struct bloc *b);
int write_inode(struct inode *i);
struct bloc create_bloc(const char *filename, const char *content);
struct bloc get_bloc_by_id(unsigned int bloc_id);
struct inode create_emptyfile(char *filename, filetype type, const char *mode);
struct inode create_inode(filetype type, mode_t perms, const char *user, const char *group);
struct inode create_root();
unsigned int get_bloc_id(char *filename);
void add_bloc(struct inode *i, struct bloc *b);
void create_disk();
void create_regularfile(char *filename, char *content);
void free_str_array(char **str_array, int len);
void init_id_generator();
void print_bloc(struct bloc *b);
void print_inode(struct inode *i);

#endif

