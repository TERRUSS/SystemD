#include "./fs.h"

const int INODE_FLAG = 1;
const int BLOC_FLAG = 2;

const mode_t DEFAULT_PERMISSIONS = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
const char ROOT[USERNAME_COUNT] = "root";
const mode_t ROOT_PERMISSIONS = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

char g_username[USERNAME_COUNT];

/* Current working directory */
struct inode g_working_directory;

/**
 * Gets the filename of a file/inode
 *
 * note: don't forget to free the char
 * on success: returns the filename
 * on failure: returns NULL
 */

void initFS(){
	// init File System
	init_id_generator();
	strcpy(g_username, "user");
}

char *get_filename_for_inode(struct inode *i) {
	struct bloc b;
	char *filename;
	filename = (char *) malloc(sizeof(char) * FILENAME_COUNT);
	filename[0] = '\0';

	return filename;
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
	b = new_bloc("");
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

/*
 * Deletes a bloc
 */
int delete_bloc(struct bloc *b) {
	struct bloc new_bloc;
	int rst;

	new_bloc = *b;
	new_bloc.id = DELETED;
	rst = overwrite_bloc(&new_bloc, b->id);
	b->id = DELETED;

	return rst;
}

/*
 * Deletes an inode
 */
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

	struct inode root;
	root = create_root();

	return root;
}

/**
 * Removes the disk file
 */
int clean_disk() {
	return remove(DISK);
}

/*
 * Get some info about the disk :
 * available blocs
 * available inodes
 * available memory (in bytes)
 */
void disk_free(unsigned int *blocs_available, unsigned int *inodes_available, size_t *bytes_available) {
	FILE *f;
	int size;
	int flag;
	struct inode i;
	struct bloc b;

	size = 0;
	*inodes_available = 0;
	*blocs_available = 0;
	f = fopen(DISK, "rb");

	do {
		size = fread(&flag, sizeof(const int), 1, f);

		if (flag == INODE_FLAG) {
			fread(&i, sizeof(struct inode), 1, f);
			if (i.id == DELETED)
				*inodes_available = *inodes_available + 1;
		} else if (flag == BLOC_FLAG) {
			fread(&b, sizeof(struct bloc), 1, f);
			if (i.id == DELETED)
				*blocs_available = *blocs_available + 1;
		} else {
			perror("Houston there's a problem with the <disk>");
		}

	} while (size != 0);

	*bytes_available = (sizeof(struct bloc) * *blocs_available)
		+ (sizeof(struct inode) * *inodes_available);

	fclose(f);
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

struct file create_regularfile(struct inode *under_dir, char *filename, char *content, int flags) {
	struct inode i;
	struct bloc b, to_update;
	struct file f;
	int z, len;
	char **blocs_contents;

	blocs_contents = NULL;
	len = strncut(&blocs_contents, content, BLOC_SIZE);

	i = new_inode(REGULAR_FILE, DEFAULT_PERMISSIONS, g_username, g_username);
	to_update = add_inode_to_inode(under_dir, &i, filename);

	for (z = 0; z != len; z++) {
		b = new_bloc(blocs_contents[z]);
		add_bloc(&i, &b);
		write_bloc(&b);
	}

	update_bloc(&to_update);
	write_inode(&i);

	free_str_array(blocs_contents, len);

	f = new_file(&i, flags);

	return f;
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

	printf("\n<<<<<<<<<<   EOF  >>>>>>>>>>\n");

	return fclose(f);
}

/*
 * Gets all inodes under a directory
 *
 * note: don't forget to free the array of inodes
 * on success: returns the length of the array of inodes
 */
int get_inodes(struct inode *under_dir, struct inode **inodes) {
	int len;
	struct bloc b;
	int z;
	int *inode_ids;

	b = get_bloc_by_id(under_dir->bloc_ids[0]);
	len = strsplt(b.content, &inode_ids, ',');
	*inodes = (struct inode *) malloc(len * sizeof(struct inode));

	for (z = 0; z != len; z++) {
		(*inodes)[z] = get_inode_by_id((unsigned int) inode_ids[z]);
	}

	free(inode_ids);

	return len;
}

/*
 * Remove an inode and his block
 */
int remove_databloc(struct inode *from_dir, char *name) {
	struct inode i;
	struct bloc b;
	int z;

	i = get_inode_by_filename(from_dir, name);
	for (z = 0; z != i.bloc_count; z++) {
		b = get_bloc_by_id(i.bloc_ids[z]);
		delete_bloc(&b);
	}

	delete_inode(&i);

	return EXIT_SUCCESS;
}

/*
 * Removes a file, any kind
 */
int remove_file(struct inode *under_dir, char *filename, enum filetype ft) {
	struct inode i;
	unsigned int file_id;

	if (under_dir->type != DIRECTORY) {
		perror("Input is not a directory");
		return EXIT_FAILURE;
	}

	/* first we remove the dir's inode and bloc */
	i = get_inode_by_filename(under_dir, filename);

	if (i.type != ft) {
		perror("Wrong file type");
		return EXIT_FAILURE;
	}

	file_id = i.id;

	if (ft == DIRECTORY) {
		/* A directory has at least the . and the .. directories */
		if (get_filecount(&i) != 2) {
			perror(DIRECTORY_NOT_EMPTY_MESSAGE);
			return EXIT_FAILURE;
		} else {
			remove_databloc(&i, "..");
			remove_databloc(&i, ".");
		}
	}
	remove_databloc(under_dir, filename);

	/* then we remove the inode from the content in under_dir's bloc */
	remove_inode_from_directory(under_dir, file_id);

	return EXIT_SUCCESS;
}

/**
 * Creates a directory
 */
struct inode create_directory(struct inode *under_dir, char *dirname) {
	struct inode i;
	struct bloc b, to_update;

	i = new_inode(DIRECTORY, DEFAULT_PERMISSIONS, g_username, g_username);
	b = new_bloc("");

	add_bloc(&i, &b);
	to_update = add_inode_to_inode(under_dir, &i, dirname);

	write_inode(&i);
	write_bloc(&b);
	update_bloc(&to_update);

	/* we add the .. dir */
	create_dot_dir(&i);
	create_dotdot_dir(under_dir, &i);

	return i;
}

/**
 * Creates a file in the filesystem
 * and returns the inode created
 *
 * filename must not be NULL
 * TODO what's the mode for ? how do you use it,
 * since you only return an inode
 */
struct file create_emptyfile(struct inode *under_dir, char *filename, enum filetype type) {
	struct bloc b, to_update;
	struct inode i;
	struct file f;

	b = new_bloc("");
	i = new_inode(type, DEFAULT_PERMISSIONS, g_username, g_username);

	add_bloc(&i, &b);
	to_update = add_inode_to_inode(under_dir, &i, filename);

	write_inode(&i);
	write_bloc(&b);
	update_bloc(&to_update);

	f = new_file(&i, O_CREAT | O_WRONLY | O_TRUNC);

	return f;
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

	if (f == NULL) {
		perror(NO_FILE_ERROR_MESSAGE);
		return i;
	}

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

	fclose(f);

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

	if (f == NULL) {
		perror(NO_FILE_ERROR_MESSAGE);
		b = empty_bloc();
		b.id = DELETED;
		return b;
	}

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

	fclose(f);

	return b;
}

/**
 * Returns the number of files under a directory
 */
unsigned int get_filecount(struct inode *dir) {
	char str[BLOC_SIZE];
	struct bloc b;

	b = get_bloc_by_id(dir->bloc_ids[0]);
	strcpy(str, b.content);
	return ocr(str, ',');
}

/**
 * Link inode to other inode (directory)
 * Returns the bloc to update (in the disk)
 */
struct bloc add_inode_to_inode(struct inode *dir, struct inode *i, char *name) {
	/* TODO out of bound */
	char str_id[15];
	struct bloc b;


	/* We assume a directory has only one bloc */
	b = get_bloc_by_id(dir->bloc_ids[0]);
	sprintf(str_id, "%d", i->id);
	strcat(b.content, str_id);
	strcat(b.content, ":");
	strcat(b.content, name);
	strcat(b.content, ",");

	return b;
}


/* Primitives */

/*
 * Write buf into an inode blocs
 * add new blocs if necessary
 * delete blocs if necessary
 */
int iwrite(struct file *f, char *buf, size_t n) {
	int z;
	int done;
	int pos;
	int new_bloc_count;
	char *filename;
	struct bloc b;
	time_t t;
	struct inode *i;

	if (!((f->flags & O_WRONLY) | (f->flags & O_RDWR))) {

		fprintf(stderr, "Access denied, wrong mode %d\n", __LINE__);
		return EXIT_FAILURE;
	}

	i = &(f->inode);
	t = time(NULL);
	done = 0;
	pos = 0;
	filename = get_filename_for_inode(i);
	z = 0;

	while (!done && z != i->bloc_count) {
		b = get_bloc_by_id(i->bloc_ids[z]);

		if (n > BLOC_SIZE - 1) {
			strncpy(b.content, buf + pos, BLOC_SIZE - 1);
			pos += BLOC_SIZE - 1;
			n -= BLOC_SIZE - 1;
		} else {
			strncpy(b.content, buf + pos, n - 1);
			b.content[n] = '\0';
			done = 1;
		}

		update_bloc(&b);
		z++;
	}

	if (done) {
		new_bloc_count = z;

		for (; z != i->bloc_count; z++) {
			b = get_bloc_by_id(i->bloc_ids[z]);
			delete_bloc(&b);
		}

		i->bloc_count = new_bloc_count;
	} else {
		while (!done) {
			if (n > strlen(b.content)) {
				b = new_bloc("");
				strncpy(b.content, buf + pos, BLOC_SIZE - 1);
				pos += BLOC_SIZE - 1;
				n -= BLOC_SIZE - 1;
			} else {
				b = new_bloc("");
				strncpy(b.content, buf + pos, n - 1);
				b.content[n] = '\0';
				done = 1;
			}

			add_bloc(i, &b);
			write_bloc(&b);
		}
	}

	free(filename);
	i->updated_at = localtime(&t);
	update_inode(i);

	return EXIT_SUCCESS;
}



int create_dot_dir(struct inode *dir) {
	struct bloc b, to_update;
	struct inode dot_dir;

	b = get_bloc_by_id(dir->bloc_ids[0]);
	dot_dir = *dir;
	dot_dir.id = rand();
	b = new_bloc(b.content);
	dot_dir.bloc_ids[0] = b.id;
	write_bloc(&b);
	to_update = add_inode_to_inode(dir, &dot_dir, ".");
	update_bloc(&to_update);
	write_inode(&dot_dir);

	return EXIT_SUCCESS;
}

int create_dotdot_dir(struct inode *parent, struct inode *dir) {
	struct bloc b, to_update;
	struct inode dotdot_dir;

	b = get_bloc_by_id(parent->bloc_ids[0]);
	dotdot_dir = *parent;
	dotdot_dir.id = rand();
	b = new_bloc(b.content);
	dotdot_dir.bloc_ids[0] = b.id;
	write_bloc(&b);
	to_update = add_inode_to_inode(dir, &dotdot_dir, "..");
	update_bloc(&to_update);
	write_inode(&dotdot_dir);

	return EXIT_SUCCESS;
}



/*
 * Returns an inode matching the filename
 *
 * exception: file not found
 * on failure: returns an empty inode
 * on success: returns the inode found
 */
struct inode get_inode_by_filename(struct inode *under_dir, char *filename) {
	struct inode i;
	struct bloc b;
	int linkcount;
	int found;
	unsigned int inode_id;
	char name[FILENAME_COUNT];
	int offset;
	int z;

	found = 0;
	i = empty_inode();
	b = get_bloc_by_id(under_dir->bloc_ids[0]);
	linkcount = ocr(b.content, ',');
	offset = 0;
	z = 0;

	while (!found && z != linkcount) {

		sscanf(b.content + sizeof(char)*offset, "%u", &inode_id);
		offset += get_index(b.content + offset, ':') + 1;
		sscanf(b.content + sizeof(char)*offset, "%s", name);
		offset += get_index(b.content + offset, ',') + 1;

		if (strcmp(name, filename) == 0) {
			i = get_inode_by_id(inode_id);
			found = 1;
		}
		z++;
	}

	/*
	len = get_inodes(under_dir, &inodes);
	z = 0;

	while (!done && z != len) {
		fn = get_filename_for_inode(inodes + z);
		if (strcmp(fn, filename) == 0) {
			i = inodes[z];
			done = 1;
		}
		free(fn);
		z++;
	}

	free(inodes);
	*/

	return i;
}

/*
 * Returns an inode of a file
 *
 * success : returns the inode
 */
struct file iopen(struct inode *under_dir, char *filename, int flags) {
	struct file f;
	struct inode i;

	i = get_inode_by_filename(under_dir, filename);

	if (i.id == DELETED && (flags & O_CREAT)) {
		f = create_emptyfile(under_dir, filename, REGULAR_FILE);
		f.flags = flags;
	} else {
		f = new_file(&i, flags);
	}

	return f;
}

/*
 * Reads n bytes of files pointed by inode i; the content
 * is stored in buf
 */
int iread(struct file *f, char *buf, size_t n) {
	int z;
	int done;
	struct bloc b;
	int pos;
	struct inode i;

	if ( ( (f->flags & O_RDONLY) == 0 && (f->flags & O_RDWR) == 0) ) {
		fprintf(stderr, "Access denied, wrong mode %d\n", __LINE__);
		return EXIT_FAILURE;
	}

	i = f->inode;
	done = 0;
	z = 0;
	pos = 0;
	strcpy(buf, "");

	while (!done && z != i.bloc_count) {
		b = get_bloc_by_id(i.bloc_ids[z]);

		if (b.id == DELETED) {
			perror(BLOC_DELETED_MESSAGE);
			continue;
		}

		if ((int) n >= 1 + (int) strlen(b.content)) {
			strcat(buf + pos, b.content);
			n -= strlen(b.content);
			pos += strlen(b.content);
		} else {
			strncat(buf + pos, b.content, n);
			strcat(buf, "\0");
			done = 1;
		}
		z++;
	}

	return EXIT_SUCCESS;
}


/*
 * TODO
 */
int iclose(struct file *f) {
	(void) f;
	return EXIT_FAILURE;
}

/**
 * List all files under a dir
 *
 * note: don't forget to free the array
 */
char **list_files(struct inode *dir, int *filecount) {
	char **files, *filename;
	int *inode_ids;
	int z;
	struct inode i;
	struct bloc b;

	files = NULL;
	b = get_bloc_by_id(dir->bloc_ids[0]);
	*filecount = strsplt(b.content, &inode_ids, ',');
	files = init_str_array(*filecount, FILENAME_COUNT);

	for (z = 0; z < *filecount; z++) {
		i = get_inode_by_id(inode_ids[z]);
		filename = get_filename_for_inode(&i);
		strncpy(files[z], filename, FILENAME_COUNT);

		free(filename);
	}

	free(inode_ids);

	return files;
}

/*
 * Remove an inode froma directory, that is,
 * remove the inode id from the bloc content
 * the ids are stored like that "12,432,4324,"
 */
int remove_inode_from_directory(struct inode *dir, unsigned int id) {
	struct bloc b;
	int found;
	unsigned int linkcount;
	int z;
	unsigned int inode_id;
	char name[FILENAME_COUNT];
	int offset, initial_offset = 0;

	found = 0;
	b = get_bloc_by_id(dir->bloc_ids[0]);
	linkcount = ocr(b.content, ',');
	offset = 0;
	initial_offset = 0;
	z = 0;

	while (!found && z != linkcount) {
		initial_offset = offset;

		sscanf(b.content + sizeof(char)*offset, "%u", &inode_id);
		offset += get_index(b.content + offset, ':') + 1;
		sscanf(b.content + sizeof(char)*offset, "%s", name);
		offset += get_index(b.content + offset, ',') + 1;

		if (inode_id == id) {
			found = 1;
		}
		/*
		if (strcmp(name, filename) == 0) {
			found = 1;
		}

		 */
		z++;
	}

	if (found) {
		offset = initial_offset;
		while (b.content != '\0') {
			b.content[offset] = b.content[offset + 1];
			offset++;
		}
	}
	/*
	b = get_bloc_by_id(dir->bloc_ids[0]);

	len = strsplt(b.content, &inode_ids, ',');
	remove_int(&inode_ids, &len, id);
	strjoin(b.content, inode_ids, len, ',');
	update_bloc(&b);

	free(inode_ids);
	*/

	return EXIT_SUCCESS;
}

/*
 * Deletes an empty directory
 *
 * exception: directory is not empty, file's not a directory
 */
int remove_empty_directory(struct inode *under_dir, char *dirname) {
	return remove_file(under_dir, dirname, DIRECTORY);
}


/*
 * Moves a file from an inode to another inode
 */
int move_file(struct inode *from, char *filename, struct inode *to) {
	struct inode i;
	struct bloc to_update;

	i = get_inode_by_filename(from, filename);

	remove_inode_from_directory(from, i.id);
	to_update = add_inode_to_inode(to, &i, filename);
	update_bloc(&to_update);

	return EXIT_SUCCESS;
}

/*
 * Copies a file from an inode to another
 */
int copy_file(struct inode *from, char *filename, char *to) {
	struct inode i, to_dir;
	struct bloc to_update;

	i = get_inode_by_filename(from, filename);

	to_dir = get_inode_by_filename(from, to);
	to_update = add_inode_to_inode(&to_dir, &i, filename);
	update_bloc(&to_update);

	return EXIT_SUCCESS;
}



/* Other */


/*
 * TODO put in utils somewhere
 * Removes an integer from an array of ints
 * Reallocates the array if the int is found (and removed)
 *
 * exception: integer not found, length is 0
 */
int remove_int(int **int_array, unsigned int *len, int i) {
	unsigned int z;
	int remove;

	if (*len == 0) {
		perror("Length is 0");
		return EXIT_FAILURE;
	}

	remove = 0;

	for (z = 0; z != *len && !remove; z++) {
		if ((*int_array)[z] == i) {
			remove = 1;
			z--;
		}
	}

	if (remove) {

		for (; z + 1 < *len; z++) {
			(*int_array)[z] = (*int_array)[z + 1];
		}
		*len -= 1;

		*int_array = (int *) realloc(*int_array, sizeof(int) * *len);
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

/*
 * TODO
 */
/*
int link_inode(struct inode *from_dir, char *filename, char *linkname) {
	struct inode link, i;
	struct bloc to_update;
	int z;

	i = get_inode_by_filename(from_dir, filename);
	link = new_inode(i.type, i.permissions, i.user_name, i.group_name);

	for (z = 0; z != i.bloc_count; z++) {
		link.bloc_ids[z] = i.bloc_ids[z];
	}

	link.bloc_count = i.bloc_count;
	write_inode(&link);

	to_update = add_inode_to_inode(from_dir, &link, filename);
	update_bloc(&to_update);

	return EXIT_SUCCESS;
}
*/

/*
 * TODO
 */
/*
int unlink_inode(struct inode *from_dir, char *linkname) {
	struct inode link;

	link = get_inode_by_filename(from_dir, linkname);
	remove_inode_from_directory(from_dir, link.id);

	return EXIT_SUCCESS;
}
*/


/* File */


/*
 * Initialize a new file
 * TODO do append flag, extremlly difficult
 */
struct file new_file(struct inode *i, int flags) {
	struct file f;

	memset(&f, 0, sizeof(struct file));
	f.inode = *i;
	f.flags = flags;

	if (flags & O_APPEND) {
		f.current_pos = get_total_strlen(i);
	} else {
		f.current_pos = 0;
	}

	return f;
}

/*
 * Returns the total length of a file
 * pointed by an inode
 */
size_t get_total_strlen(struct inode *i) {
	struct bloc b;
	int bloc_count;

	bloc_count = i->bloc_count;
	b = get_bloc_by_id(i->bloc_ids[bloc_count - 1]);

	return (bloc_count - 1) * (BLOC_SIZE - 1) + strlen(b.content);
}

void ch_dir(unsigned int inodeid){

	char * buff = malloc(sizeof(char) * 100);
	sprintf(buff, "%u", inodeid);

	setenv("SYSD_CURDIR", buff, 1);/* 1 is for overwrite */
}

char * get_filename_for_inodeID(unsigned int id) {
	struct inode i = get_inode_by_id(id);
	char * fn = get_filename_for_inode( &i );
	return fn;
}

unsigned int get_pwd_id(){
	//void * cur = malloc(sizeof(unsigned int));
	unsigned int cur;
	sscanf(getenv("SYSD_CURDIR"), "%u", &cur);

	return cur;
}

void update_path(unsigned int inodeid){
	key_t key = ftok("systemd",65);
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    char *shared = (char*) shmat(shmid,(void*)0,0);

    sprintf(shared, "%u", inodeid);

    shmdt(shared);
    shmctl(shmid,IPC_RMID,NULL);
}
