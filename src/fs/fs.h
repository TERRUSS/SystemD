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

#define PRINT_LINE printf("LINE %d\n", __LINE__)

#define DISK "rsc/disk"

extern const int INODE_FLAG;
extern const int BLOC_FLAG;

extern struct inode g_working_directory;
extern struct file g_filetree;

struct file {
	struct inode i;
	struct bloc b;
	struct file *files;
	int files_count;
};

char *get_filename_for_inode(struct inode *i);
int clean_disk();
int delete_inode(struct inode *i);
int overwrite_inode(struct inode *new_inode, unsigned int id);
int print_disk();
int update_inode(struct inode *new_inode);
int overwrite_bloc(struct bloc *new_bloc, unsigned int id);
int update_bloc(struct bloc *new_bloc);
int write_bloc(struct bloc *b);
int write_inode(struct inode *i);
struct bloc *get_inode_blocs(struct inode *i);
struct bloc add_inode_to_inode(struct inode *dir, struct inode *i);
struct bloc get_bloc_by_id(unsigned int bloc_id);
struct inode create_disk();
struct inode create_emptyfile(char *filename, filetype type, const char *mode);
struct inode create_regularfile(char *filename, char *content);
struct inode create_directory(struct inode *under_dir, char *dirname);
struct inode create_root();
unsigned int get_bloc_id(char *filename);
void free_str_array(char **str_array, int len);
void inode_count(unsigned int *in_store, unsigned int *deleted);
unsigned int get_filecount(struct inode *dir);

void iwrite(struct inode *i, char *buf);

#endif

