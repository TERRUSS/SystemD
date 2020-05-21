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
#include "utils/str_utils.h"
#include "fs/inode.h"
#include "fs/bloc.h"

/* Macros */

#define DISK "rsc/disk"

/* Constants */

extern const int INODE_FLAG;
extern const int BLOC_FLAG;

/* Globals */

extern struct inode g_current_node;
extern struct file g_filetree;

/* Structures */


struct file {
	struct inode i;
	struct bloc b;
	struct file *files;
	int files_count;
};

/* Prototypes */

int clean_disk();
int print_disk();
int update_bloc_content(unsigned int bloc_id, const char *new_content);
int update_inode(struct inode *new_inode);
int write_bloc(struct bloc *b);
int write_inode(struct inode *i);
struct bloc *get_inode_blocs(struct inode *i);
struct bloc get_bloc_by_id(unsigned int bloc_id);
struct inode create_emptyfile(char *filename, filetype type, const char *mode);
struct inode create_regularfile(char *filename, char *content);
struct inode create_root();
unsigned int get_bloc_id(char *filename);
void create_disk();
void free_str_array(char **str_array, int len);
void iwrite(struct inode *i, char *buf);
struct bloc add_inode_to_inode(struct inode *dir, struct inode *i);

#endif

