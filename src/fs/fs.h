#ifndef FS_H
#define FS_H

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "../utils/str_utils.h"
#include "./inode.h"
#include "./bloc.h"
#include <sys/ipc.h> 
#include <sys/shm.h> 

#define PRINT_LINE printf("LINE %d\n", __LINE__)
#define NO_FILE_ERROR_MESSAGE "File's NULL"
#define BLOC_DELETED_MESSAGE "Bloc's deleted (id == 0)"
#define DIRECTORY_NOT_EMPTY_MESSAGE "Directory's not empty"

#define DISK "rsc/disk"

extern const int INODE_FLAG;
extern const int BLOC_FLAG;

extern const mode_t DEFAULT_PERMISSIONS;
extern const char ROOT[USERNAME_COUNT];
extern const mode_t ROOT_PERMISSIONS;
extern char g_username[USERNAME_COUNT];
extern struct inode g_working_directory;

/*
 * Composed of an inode and some flags to retreit access to the file
 *
 * according to man fopen :
 * r   O_RDONLY
 * w   O_WRONLY | O_CREAT | O_TRUNC
 * a   O_WRONLY | O_CREAT | O_APPEND
 * r+  O_RDWR
 * w+  O_RDWR | O_CREAT | O_TRUNC
 * a+  O_RDWR | O_CREAT | O_APPEND
 */
struct file {
	struct inode inode;
	int flags;
	int current_pos;
};

struct file new_file(struct inode *i, int flags);
size_t get_total_strlen(struct inode *i);

void initFS();
char * get_filename_for_inode(struct inode *i);
int clean_disk();
int create_dot_dir(struct inode *dir);
int create_dotdot_dir(struct inode *parent, struct inode *dir);
int delete_bloc(struct bloc *b);
int delete_inode(struct inode *i);
int get_inodes(struct inode *under_dir, struct inode **inodes);
int overwrite_bloc(struct bloc *new_bloc, unsigned int id);
int overwrite_inode(struct inode *new_inode, unsigned int id);
int print_disk();
int remove_inode_from_directory(struct inode *dir, unsigned int id);
int update_bloc(struct bloc *new_bloc);
int update_inode(struct inode *new_inode);
int write_bloc(struct bloc *b);
int write_inode(struct inode *i);
struct bloc add_inode_to_inode(struct inode *dir, struct inode *i);
struct bloc get_bloc_by_id(unsigned int bloc_id);
struct inode create_disk();
struct inode create_root();
struct inode get_inode_by_filename(struct inode *under_dir, char *filename);
struct inode get_inode_by_id(unsigned int inode_id);
unsigned int get_filecount(struct inode *dir);
void disk_free(unsigned int *blocs_available, unsigned int *inodes_available, size_t *bytes_available);

char **list_files(struct inode *dir, int *filecount);
int copy_file(struct inode *from, char *filename, char *to);
int iread(struct file *f, char *buf, size_t n);
int iwrite(struct file *f, char *buf, size_t n);
int link_inode(struct inode *from_dir, char *filename, char *linkname);
int move_file(struct inode *from, char *filename, struct inode *to);
int unlink_inode(struct inode *from_dir, char *linkname);
struct file create_emptyfile(struct inode *under_dir, char *filename, enum filetype type);
struct file create_regularfile(struct inode *under_dir, char *filename, char *content, int flags);
struct file iopen(struct inode *under_dir, char *filename, int flags);

struct inode create_directory(struct inode *under_dir, char *dirname);
int remove_empty_directory(struct inode *under_dir, char *dirname);
int remove_file(struct inode *under_dir, char *filename, enum filetype ft);

int remove_int(int **int_array, unsigned int *len, int i);
void ch_dir(unsigned int inodeid);
char * get_filename_for_inodeID(unsigned int id);
unsigned int get_pwd_id();
void update_path(unsigned int inodeid);
#endif