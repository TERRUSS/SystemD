#include "fs/fs.h"

const int INODE_FLAG = 1;
const int BLOC_FLAG = 2;

const mode_t DEFAULT_PERMISSIONS = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
const char ROOT[USERNAME_COUNT] = "root";
const mode_t ROOT_PERMISSIONS = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

char g_username[USERNAME_COUNT];

/* Current working directory */
struct inode g_working_directory;
/* Contains the filetree TODO will be used ? */
struct file g_filetree;

/**
 * Gets the filename of a file/inode
 *
 * note: don't forget to free the char
 * on success: returns the filename
 * on failure: returns NULL
 */
char *get_filename_for_inode(struct inode *i) {
	if (i->bloc_count == 0) {
		perror("The inode has no blocs");
		return NULL;
	}

	return get_bloc_by_id(i->bloc_ids[0]).filename;
}

/**
 * The root has an id == 1, and an empty filename
 * Written on the disk
 *
 * Returns the inode created
 */
struct inode create_root() {
	struct inode i;
	struct bloc b;

	i = new_inode(DIRECTORY, ROOT_PERMISSIONS, ROOT, ROOT);
	b = new_bloc("", "");
	i.id = ROOT_ID;

	add_bloc(&i, &b);

	write_inode(&i);
	write_bloc(&b);

	return i;
}

/**
 * Writes an inode to the disk (by append)
 *
 * on failure: returns 0
 * on success: returns 1
 */
int write_inode(struct inode *i) {
	FILE *f;

	if (overwrite_inode(i, DELETED) == EXIT_SUCCESS) {
		printf("Inode overwritten\n");
		return EXIT_SUCCESS;
	}

	f = fopen(DISK, "ab");

	if (f == NULL) {
		fprintf(stderr, "File's NULL %d", __LINE__);
		return EXIT_FAILURE;
	}

	fwrite(&INODE_FLAG, sizeof(INODE_FLAG), 1, f);
	fwrite(i, sizeof(struct inode), 1, f);

	fclose(f);
	return EXIT_SUCCESS;
}

int delete_inode(struct inode *i) {
	struct inode new_inode;
	int rst;

	new_inode = *i;
	new_inode.id = DELETED;
	rst = overwrite_inode(&new_inode, i->id);
	i->id = DELETED;

	return rst;
}

/**
 * Creates the root inode and write to a file
 * Call only once
 */
struct inode create_disk() {
	FILE *f;

	f = fopen(DISK, "ab+");
	fclose(f);
	return create_root();
}

/**
 * Removes the disk file
 */
int clean_disk() {
	return remove(DISK);
}

/**
 * TODO
 * Get some info about the disk :
 * available blocs
 * available inodes
 * available memory (in bytes)
 */
void disk_free(unsigned int *blocs_available, unsigned int *inodes_available, size_t bytes_available) {
}

/**
 * Overwrite an inode by its id
 *
 * on success : returns 1
 * on failure : returns 0
 */
int overwrite_inode(struct inode *new_inode, unsigned int id) {
	FILE *f;
	int size;
	int flag;
	int pos;
	struct inode i;
	int updated;
	struct bloc b;

	size = 0;
	updated = 0;
	f = fopen(DISK, "r+b");

	if (f == NULL) {
		fprintf(stderr, "File empty %d", __LINE__);
		return EXIT_FAILURE;
	}

	do {
		/*
		 * We determine if it's a bloc or an inode by the flag
		 */
		size = fread(&flag, sizeof(const int), 1, f);
		pos = ftell(f);

		if (size == 0) continue;

		if (flag == INODE_FLAG) {
			fread(&i, sizeof(struct inode), 1, f);

			if (i.id == id) {
				fseek(f, pos, SEEK_SET);
				fwrite(new_inode, sizeof(struct inode), 1, f);
				updated = 1;
			}
		} else {
			fread(&b, sizeof(struct bloc), 1, f);
		}

	} while (size != 0 && !updated);

	fclose(f);

	if (updated) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}

int overwrite_bloc(struct bloc *new_bloc, unsigned int id) {
	FILE *f;
	int size;
	int flag;
	int pos;
	struct bloc i;
	int updated;
	struct bloc b;

	size = 0;
	updated = 0;
	f = fopen(DISK, "r+b");

	if (f == NULL) {
		fprintf(stderr, "File empty %d", __LINE__);
		return EXIT_FAILURE;
	}

	do {
		size = fread(&flag, sizeof(const int), 1, f);
		pos = ftell(f);

		if (size == 0) continue;

		if (flag == BLOC_FLAG) {
			fread(&b, sizeof(struct bloc), 1, f);

			if (b.id == id) {
				fseek(f, pos, SEEK_SET);
				fwrite(new_bloc, sizeof(struct bloc), 1, f);
				updated = 1;
			}
		} else {
			fread(&i, sizeof(struct inode), 1, f);
		}

	} while (size != 0 && !updated);

	fclose(f);

	if (updated) {
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}

/**
 * Updates an inode in the disk file
 * DEPRECATED use overwrite_inode instead
 *
 * on success : returns 1
 * on failure : returns 0
 */
int update_inode(struct inode *new_inode) {
	return overwrite_inode(new_inode, new_inode->id);
}

/**
 * TODO fix redundancy
 */
int update_bloc(struct bloc *new_bloc) {
	return overwrite_bloc(new_bloc, new_bloc->id);
}

struct inode create_regularfile(struct inode *under_dir, char *filename, char *content) {
	struct inode i;
	struct bloc b, to_update;
	int z, len;
	char **blocs_contents;

	blocs_contents = NULL;
	len = strncut(&blocs_contents, content, BLOC_SIZE);

	i = new_inode(REGULAR_FILE, DEFAULT_PERMISSIONS, g_username, g_username);
	to_update = add_inode_to_inode(under_dir, &i);

	for (z = 0; z != len; z++) {
		b = new_bloc(filename, blocs_contents[z]);
		add_bloc(&i, &b);
		write_bloc(&b);
	}

	update_bloc(&to_update);
	write_inode(&i);

	free_str_array(blocs_contents, len);

	return i;
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
	if (f == NULL) {
		fprintf(stderr, "File's NULL %d\n", __LINE__);
		return EXIT_FAILURE;
	}

	printf("<<<<<<<<<< DISK >>>>>>>>>>\n");

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

	printf("<<<<<<<<<<      >>>>>>>>>>\n");

	return fclose(f);
}

/**
 * Creates a directory
 */
struct inode create_directory(struct inode *under_dir, char *dirname) {
	struct inode i;
	struct bloc b, to_update;

	i = new_inode(DIRECTORY, DEFAULT_PERMISSIONS, g_username, g_username);
	b = new_bloc(dirname, "");

	add_bloc(&i, &b);
	to_update = add_inode_to_inode(under_dir, &i);

	write_inode(&i);
	write_bloc(&b);
	update_bloc(&to_update);

	return i;
}

/**
 * Created a file in the filesystem
 * and returns the inode created
 *
 * filename must not be NULL
 * TODO what's the mode for ? how do you use it,
 * since you only return an inode
 */
struct inode create_emptyfile(struct inode *under_dir, char *filename, filetype type, const char *mode) {
	struct bloc b, to_update;
	struct inode i;

	b = new_bloc(filename, "");
	i = new_inode(type, DEFAULT_PERMISSIONS, g_username, g_username);

	add_bloc(&i, &b);
	to_update = add_inode_to_inode(under_dir, &i);

	write_inode(&i);
	write_bloc(&b);
	update_bloc(&to_update);

	return i;
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
 * Returns an inode by its id
 */
struct inode get_inode_by_id(unsigned int inode_id) {
	FILE *f;
	int size;
	int flag;
	struct bloc b;
	struct inode i;
	int match = 0;

	size = 0;
	f = fopen(DISK, "rb");

	do {
		size = fread(&flag, sizeof(const int), 1, f);

		if (size == 0) continue;

		if (flag == INODE_FLAG) {
			fread(&i, sizeof(struct inode), 1, f);
			if (i.id == inode_id) {
				match = !match;
			}
		} else {
			fread(&b, sizeof(struct bloc), 1, f);
		}

	} while (size != 0 && !match);

	return i;
}

/**
 * Returns a bloc by its id
 */
struct bloc get_bloc_by_id(unsigned int bloc_id) {
	FILE *f;
	int size;
	int flag;
	struct bloc b;
	struct inode i;
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
		} else {
			fread(&i, sizeof(struct inode), 1, f);
		}

	} while (size != 0 && !match);

	return b;
}

/**
 */
struct bloc *get_inode_blocs(struct inode *i) {
	int z;
	struct bloc *blocs;

	blocs = (struct bloc *) calloc(i->bloc_count, sizeof(struct bloc));

	for (z = 0; z != i->bloc_count; z++) {
		blocs[z] = get_bloc_by_id(i->bloc_ids[z]);
	}

	return blocs;
}

/**
 * Returns the number of files under a directory
 */
unsigned int get_filecount(struct inode *dir) {
	char str[BLOC_SIZE];

	strcpy(str, get_bloc_by_id(dir->bloc_ids[0]).content);
	return ocr(str, ',');
}

/**
 * Link inode to other inode (directory)
 * Returns the bloc to update (in the disk)
 */
struct bloc add_inode_to_inode(struct inode *dir, struct inode *i) {
	/* TODO out of bound */
	char str_id[15];
	struct bloc b;


	/* We assume a directory has only one bloc */
	b = get_bloc_by_id(dir->bloc_ids[0]);
	sprintf(str_id, "%d", i->id);
	strcat(b.content, str_id);
	strcat(b.content, ",");

	return b;
}

/**
 * Counts number of inodes used and deleted in the disk
 */
void inode_count(unsigned int *in_store, unsigned int *deleted) {
	FILE *f;
	int size;
	int flag;
	struct inode i;
	struct bloc b;

	size = 0;
	*in_store = 0;
	*deleted = 0;
	f = fopen(DISK, "r+b");

	do {
		size = fread(&flag, sizeof(const int), 1, f);

		if (flag == INODE_FLAG) {
			fread(&i, sizeof(struct inode), 1, f);
			if (i.id == DELETED)
				*deleted = *deleted +1;
			else
				*in_store = *in_store + 1;
		} else {
			fread(&b, sizeof(struct bloc), 1, f);
		}

	} while (size != 0);

	fclose(f);
}

/* Primitives */

/**
 * TODO not working, segfault
 */
void iwrite(struct inode *i, char *buf) {
	struct bloc *blocs, b;
	char **contents;
	char *filename;
	int len;
	int z;

	blocs = get_inode_blocs(i);
	filename = blocs[0].filename;
	len = strncut(&contents, buf, BLOC_SIZE);

	if (i->bloc_count < len) {
		printf("__LINE__ %d\n", __LINE__);
		for (z = 0; z != i->bloc_count; z++) {
			strncpy(blocs[z].content, contents[z], BLOC_SIZE);
			printf("__LINE__ %d\n", __LINE__);
			print_bloc(blocs + z);
			printf("SIZE %lu\n", strlen(blocs[z].content));
			update_bloc(blocs + z);// segfault
		}

		for (; z != len; z++) {
			b = new_bloc(filename, contents[z]);
			print_bloc(blocs + z);
			write_bloc(&b);
			add_bloc(i, &b);
		}
		TODO_PRINT;
		free_str_array(contents, len);
	} else {
		printf("__LINE__ %d\n", __LINE__);
		for (z = 0; z != len; z++) {
			strncpy(blocs[z].content, contents[z], BLOC_SIZE);
			update_bloc(blocs + z);
		}
		TODO_PRINT;

		for (; z != i->bloc_count; z++) {
		}
		free_str_array(contents, i->bloc_count);
	}

	update_inode(i);
	free(blocs);
}

/**
 * Returns an inode of a file
 *
 * success : returns the inode
 */
struct inode iopen(char *filename, const char *mode) {
	struct inode i;

	return i;
}

// TODO
int iread(struct inode *i, char *buf, size_t len) {
	return EXIT_FAILURE;
}


// TODO
int iclose(struct inode *i) {
	return EXIT_FAILURE;
}

// TODO
/**
 * List all files under a dir
 *
 * note: don't forget to free the array
 */
char **list_files(struct inode *dir, int *filecount) {
	char **files;
	unsigned int *inode_ids;
	int z;
	struct inode i;

	files = NULL;
	*filecount = get_filecount(dir);
	inode_ids = parse_ids(get_bloc_by_id(dir->bloc_ids[0]).content);
	files = init_str_array(*filecount, FILENAME_COUNT);

	for (z = 0; z != *filecount; z++) {
		i = get_inode_by_id(inode_ids[z]);
		strncpy(files[z], get_filename_for_inode(&i), FILENAME_COUNT + 1);
	}

	free(inode_ids);

	return files;
}

