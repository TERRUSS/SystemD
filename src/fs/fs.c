#include "fs/fs.h"

const int INODE_FLAG = 1;
const int BLOC_FLAG = 2;
const mode_t DEFAULT_PERMISSIONS = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
const char USERNAME[USERNAME_COUNT] = "macron";
struct inode g_current_node;
struct file g_filetree;
unsigned int g_increment_id = 1;

/**
 * Returns an inode
 *
 * user mut not be NULL
 * group can be NULL
 *
 * on success : returns inode
 * on failure : returns NULL
 */
struct inode create_inode(filetype type, mode_t perms, const char *user, const char *group) {

	struct inode i;
	time_t t;

	//i.id = rand();
	i.id = g_increment_id++;

	i.type = type;
	i.permissions = perms;

	strcpy(i.user_name, user);
	if (group != NULL)
		strcpy(i.group_name, group);
	else
		strcpy(i.group_name, user);


	time(NULL);
	i.created_at = localtime(&t);
	i.updated_at = localtime(&t);

	i.bloc_count = 0;

	return i;
}

/**
 * The root has an id == 1, and an empty filename
 * Written on the disk
 */
struct inode create_root() {
	struct inode i;
	struct bloc b;
	mode_t root_permissions = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

	i = create_inode(DIRECTORY, root_permissions, "root", "root");
	b = create_bloc("", "");
	b.id = g_increment_id++;

	add_bloc(&i, &b);

	write_inode(&i);
	write_bloc(&b);

	return i;
}

/**
 * Adds a bloc id to an inode
 */
void add_bloc(struct inode *i, struct bloc *b) {
	if (i->bloc_count == BLOC_IDS_COUNT) {
		perror("Can't add anymore blocs to the inode !\n");
	} else {
		i->bloc_ids[i->bloc_count] = b->id;
		i->bloc_count++;
	}
}

/**
 * Checks if a bloc id is in the bloc ids of an inode
 *
 * success : 1
 * failure : 0
 */
int contains(struct inode *i, unsigned int bloc_id) {
	int j;

	for (j = 0; j != i->bloc_count; j++) {
		if (i->bloc_ids[j] == bloc_id)
			return 1;
	}

	return 0;
}

void print_bloc(struct bloc *b) {
	printf("bloc id:%d", b->id);
	printf(" filename:%s", b->filename);
	printf(" content:%s", b->content);
	puts("");
}

/**
 * Prints an inode in the terminal
 */
void print_inode(struct inode *i) {
	char s[64];
	char s2[64];
	int j;

	printf("inode id:%d", i->id);
	printf(" filetype:%d", i->type);
	printf(" permissions:%d", i->permissions);
	printf(" user:%s", i->user_name);
	printf(" group:%s", i->group_name);
	assert(strftime(s, sizeof(s), "%c", i->created_at));
	assert(strftime(s2, sizeof(s2), "%c", i->updated_at));
	printf(" created at:%s", s);
	printf(" updated at:%s", s2);

	for (j = 0; j != i->bloc_count; j++) {
		printf(" bloc_id:%d", i->bloc_ids[j]);
	}

	puts("");
}

/**
 * Writes an inode to the disk (by append)
 *
 * Returns fclose return value
 */
int write_inode(struct inode *i) {
	FILE *f;

	f = fopen(DISK, "ab");

	fwrite(&INODE_FLAG, sizeof(const int), 1, f);
	fwrite(i, sizeof(struct inode), 1, f);

	return fclose(f);
}

/**
 * Returns a bloc
 *
 * filename must not be NULL
 * content can be NULL
 */
struct bloc create_bloc(const char *filename, const char *content) {
	struct bloc b;

	//b.id = rand();
	b.id = g_increment_id++;

	// TODO strncpy for safer copy
	strcpy(b.filename, filename);

	if (content == NULL) {
		strcpy(b.content, "");
	} else {
		strcpy(b.content, content);
	}

	return b;
}

/**
 * Writes a bloc to the disk (by append)
 *
 * Returns fclose return value
 */
int write_bloc(struct bloc *b) {
	FILE *f;

	f = fopen(DISK, "ab");

	fwrite(&BLOC_FLAG, sizeof(const int), 1, f);
	fwrite(b, sizeof(struct bloc), 1, f);

	return fclose(f);
}

/**
 * Prints the disk in the terminal, inodes and blocs alike
 *
 * Returns fclose return value
 */
int print_disk() {
	FILE *f;
	int size;
	int flag;
	struct bloc b;
	struct inode i;

	size = 0;
	f = fopen(DISK, "rb");

	do {
		/*
		 * We determine if it's a bloc or an inode by the flag
		 */
		size = fread(&flag, sizeof(const int), 1, f);

		if (size == 0) continue;

		if (flag == BLOC_FLAG) {
			fread(&b, sizeof(struct bloc), 1, f);
			print_bloc(&b);

		} else if (flag == INODE_FLAG) {
			fread(&i, sizeof(struct inode), 1, f);
			print_inode(&i);

		} else {
			printf("?\n");
		}

	} while (size != 0);

	return fclose(f);
}

/**
 * Created a file in the filesystem
 * and returns the inode created
 *
 * filename must not be NULL
 * TODO what's the mode for ? how do you use it,
 * since you only return an inode
 */
struct inode create_file(char *filename, filetype type, const char *mode) {
	struct bloc b;
	struct inode i;

	b = create_bloc(filename, "");
	i = create_inode(type, DEFAULT_PERMISSIONS, USERNAME, USERNAME);

	// we link the bloc to the inode
	add_bloc(&i, &b);

	write_inode(&i);
	write_bloc(&b);

	return i;
}

/**
 * TODO TO TEST
 * TODO To avoid conflict named it iopen
 * TODO open a file under a directory (here, opens any kind of file)
 * Returns an inode of a file
 *
 * success : returns the inode
 * failure : returns empty node TODO is_empty(inode)
 */
struct inode iopen(char *filename, const char *mode) {
	struct inode i;

	return i;
}

// TODO
int iread(struct inode *i, char *buf, size_t len) {
	return 0;
}

// TODO
int iwrite(struct inode *i, char *buf, size_t len) {
	return 0;
}

// TODO
int iclose(struct inode *i) {
	return 0;
}

// TODO
char **list_files(struct inode *i) {
	int j;
	char **filenames;
	struct bloc b;

	filenames = (char **) malloc(sizeof(char *) * i->bloc_count);

	// for each bloc id
	for (j = 0; j != i->bloc_count; j++) {
		// we search in the file for the bloc id
		b = get_bloc_by_id(i->bloc_ids[j]);
		filenames[j] = (char *) malloc(sizeof(char) * FILENAME_COUNT);
		// and then we take out the filename
		strcpy(filenames[j], b.filename);
	}

	return filenames;
}

struct bloc get_bloc_by_id(unsigned int bloc_id) {
	FILE *f;
	int size;
	int flag;
	struct bloc b;
	int match = 0;

	size = 0;
	f = fopen(DISK, "rb");

	do {
		size = fread(&flag, sizeof(const int), 1, f);

		if (size == 0) continue;

		if (flag == BLOC_FLAG) {
			fread(&b, sizeof(struct bloc), 1, f);
			if (b.id == bloc_id) {
				match = !match;
			}

		}

	} while (size != 0 && !match);

	return b;
}

// TODO
void disk_free(unsigned int *blocs_available, unsigned int *inodes_available, size_t bytes_available) {
}


/**
 * Creates the root inode and write to a file
 * Call only once
 */
void create_disk() {
    create_root();
}

/**
 * Reads the disk file, and fill the filetree with disk datas
 */
void get_filetree() {
}

int update_inode(struct inode *new_inode) {
	FILE *f;
	int size;
	int flag;
	int pos;
	struct inode i;

	size = 0;
	f = fopen(DISK, "r+b");

	do {
		/*
		 * We determine if it's a bloc or an inode by the flag
		 */
		size = fread(&flag, sizeof(const int), 1, f);
		pos = ftell(f);

		if (size == 0) continue;

		if (flag == INODE_FLAG) {
			fread(&i, sizeof(struct inode), 1, f);

			if (new_inode->id == i.id) {
				fseek(f, pos, SEEK_SET);
				fwrite(new_inode, sizeof(struct inode), 1, f);
			}

		}

	} while (size != 0);

	return fclose(f);
}

int clean_disk() {
	return remove(DISK);
}


