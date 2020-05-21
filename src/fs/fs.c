#include "fs/fs.h"

const int INODE_FLAG = 1;
const int BLOC_FLAG = 2;

/* Current working directory */
struct inode g_current_node;
/* Contains the filetree TODO will be used ? */
struct file g_filetree;

/**
 * The root has an id == 1, and an empty filename
 * Written on the disk
 *
 * Returns the inode created
 */
struct inode create_root() {
	struct inode i;
	struct bloc b;

	i = create_inode(DIRECTORY, ROOT_PERMISSIONS, ROOT, ROOT);
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
 * Returns fclose return value
 */
int write_inode(struct inode *i) {
	FILE *f;

	f = fopen(DISK, "ab");

	if (f == NULL) {
		perror("Can't open file");
		return 0;
	}

	fwrite(&INODE_FLAG, sizeof(const int), 1, f);
	fwrite(i, sizeof(struct inode), 1, f);

	fclose(f);
	return 1;
}

/**
 * Creates the root inode and write to a file
 * Call only once
 */
void create_disk() {
	create_root();
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
 * TODO will be used?
 * Reads the disk file, and fill the filetree with disk datas
 */
void get_filetree() {
}

/**
 * Updates an inode in the disk file
 */
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

	fclose(f);
	return 1;
}

struct inode create_regularfile(char *filename, char *content) {
	struct inode i;
	struct bloc b;
	int z;
	int len;
	char **blocs_contents;

	blocs_contents = NULL;
	len = strncut(&blocs_contents, content, BLOC_SIZE);

	i = create_inode(REGULAR_FILE, DEFAULT_PERMISSIONS, g_username, g_username);


	for (z = 0; z != len - 1; z++) {
		b = new_bloc(filename, blocs_contents[z]);
		add_bloc(&i, &b);
		write_bloc(&b);
	}

	b = new_bloc(filename, blocs_contents[z]);
	/*b.last_bloc = LAST_BLOC;*/
	add_bloc(&i, &b);
	write_bloc(&b);

	write_inode(&i);

	free_str_array(blocs_contents, len);

	return i;
}

/**
 * Updates a bloc's content in the disk file
 */
int update_bloc_content(unsigned int bloc_id, const char *new_content) {
	FILE *f;
	int size;
	int flag;
	int pos;
	struct bloc b;

	size = 0;
	f = fopen(DISK, "r+b");

	do {
		size = fread(&flag, sizeof(const int), 1, f);
		pos = ftell(f);

		if (size == 0) continue;

		if (flag == BLOC_FLAG) {
			fread(&b, sizeof(struct bloc), 1, f);

			if (bloc_id == b.id) {
				fseek(f, pos, SEEK_SET);
				strcpy(b.content, new_content);
				fwrite(&b, sizeof(struct bloc), 1, f);
			}

		}

	} while (size != 0);

	return fclose(f);
}


/**
 * Updates a bloc in the disk file
 * Before calling the function, check the content is < 1024
 * if not, create a new for the inode
 */
int update_bloc(struct bloc *new_bloc) {
	FILE *f;
	int size;
	int flag;
	int pos;
	struct inode b;

	if (new_bloc == NULL) {
		perror("Bloc's NULL");
		return 0;
	}

	size = 0;
	f = fopen(DISK, "r+b");

	if (f == NULL) {
		perror("Can't open file");
		return 0;
	}

	do {
		/*
		 * We determine if it's a bloc or an inode by the flag
		 */
		size = fread(&flag, sizeof(const int), 1, f);
		pos = ftell(f);

		if (size == 0) continue;

		if (flag == BLOC_FLAG) {
			fread(&b, sizeof(struct bloc), 1, f);

			if (new_bloc->id == b.id) {
				fseek(f, pos, SEEK_SET);
				fwrite(new_bloc, sizeof(struct bloc), 1, f);
			}

		}

	} while (size != 0);

	fclose(f);
	return 1;
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
struct inode create_directory(char *dirname) {
	struct inode i;
	struct bloc b;

	i = create_inode(DIRECTORY, DEFAULT_PERMISSIONS, g_username, g_username);
	b = new_bloc(dirname, "");

	add_bloc(&i, &b);

	write_inode(&i);
	write_bloc(&b);

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
struct inode create_emptyfile(char *filename, filetype type, const char *mode) {
	struct bloc b;
	struct inode i;

	b = new_bloc(filename, "");
	i = create_inode(type, DEFAULT_PERMISSIONS, g_username, g_username);

	// we link the bloc to the inode
	add_bloc(&i, &b);

	write_inode(&i);
	write_bloc(&b);

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
 * TODO
 */
struct inode *get_inodes(struct inode *i) {
	struct inode *inodes;
	int z;

	inodes = NULL;
	//inodes = (struct inode *) malloc(sizeof(struct inode));

	for (z = 0; z != i->bloc_count; z++) {
	}

	return inodes;
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

/* Primitives */

/**
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
			/*blocs[z].last_bloc = NOT_LAST_BLOC;*/
			printf("__LINE__ %d\n", __LINE__);
			print_bloc(blocs + z);
			printf("SIZE %lu\n", strlen(blocs[z].content));
			update_bloc(blocs + z);
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
			/*blocs[z].last_bloc = NOT_LAST_BLOC;*/
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
 * TODO TO TEST
 * TODO To avoid conflict named it iopen
 * TODO open a file under a directory (here, opens any kind of file)
 * Returns an inode of a file
 *
 * success : returns the inode
 * failure : TODO
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
int iclose(struct inode *i) {
	return 0;
}

// TODO
char **list_files(struct inode *i) {
	char **files;
	files = NULL;
	return files;
}
