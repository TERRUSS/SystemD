#include "fs/fs.h"

/* Constants */

const int INODE_FLAG = 1;
const int BLOC_FLAG = 2;
const mode_t DEFAULT_PERMISSIONS = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
const char USERNAME[USERNAME_COUNT] = "macron";
const char ROOT[USERNAME_COUNT] = "root";
const mode_t ROOT_PERMISSIONS = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

/* Globals */

/* Current working directory */
struct inode g_current_node;
/* Contains the filetree TODO will be used ? */
struct file g_filetree;

/* Functions */

/* Inode */

/**
 * Returns an inode
 *
 * user must not be NULL
 * group can be NULL
 *
 * on success : returns inode
 * on failure : TODO
 */
struct inode create_inode(filetype type, mode_t perms, const char *user, const char *group) {

	struct inode i;
	time_t t;

	i.id = rand();

	i.type = type;
	i.permissions = perms;

	strcpy(i.user_name, user);
	if (group != NULL)
		strcpy(i.group_name, group);
	else
		strcpy(i.group_name, user);


	t = time(NULL);
	i.created_at = localtime(&t);
	i.updated_at = localtime(&t);

	i.bloc_count = 0;

	return i;
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

	i = create_inode(DIRECTORY, ROOT_PERMISSIONS, ROOT, ROOT);
	b = new_bloc("", "");
	i.id = ROOT_ID;

	add_bloc(&i, &b);

	write_inode(&i);
	write_bloc(&b);

	return i;
}


/**
 * Adds a bloc id to an inode
 * TODO add return code
 * on success : returns 1
 * on failure : returns 0
 */
void add_bloc(struct inode *i, struct bloc *b) {
	if (i->bloc_count == BLOC_IDS_COUNT) {
		perror("Can't add anymore blocs to the inode !");
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

/**
 * Prints an inode in the terminal
 */
void print_inode(struct inode *i) {
	char s[64];
	char s2[64];
	int j;

	printf("<INODE> id:%d", i->id);
	printf(" filetype:%d", i->type);
	printf(" permissions:%d", i->permissions);
	printf(" user:%s", i->user_name);
	printf(" group:%s", i->group_name);
	assert(strftime(s, sizeof(s), "%c", i->created_at));
	assert(strftime(s2, sizeof(s2), "%c", i->updated_at));
	printf(" created at:%s", s);
	printf(" updated at:%s", s2);

	puts("");
	for (j = 0; j != i->bloc_count; j++) {
		printf("\tbloc_id:%d\n", i->bloc_ids[j]);
	}

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


/* Bloc */

/**
 * Returns a bloc
 *
 * filename must not be NULL (except for root)
 * content can be NULL
 */
struct bloc new_bloc(const char *filename, const char *content) {
	struct bloc b;

	b.id = rand();

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
 * Deletes a bloc : set the id to DELETED (0)
 */
void delete_bloc(struct bloc *b) {
	b->id = DELETED;
}

/**
 * Prints a bloc to the terminal
 */
void print_bloc(struct bloc *b) {
	printf("<BLOC> id:%d", b->id);
	printf(" filename:%s", b->filename);
	printf(" content:%s", b->content);
	puts("");
}


/* Disk */

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

	i = create_inode(REGULAR_FILE, DEFAULT_PERMISSIONS, USERNAME, USERNAME);


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
	i = create_inode(type, DEFAULT_PERMISSIONS, USERNAME, USERNAME);

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

/* Utils */

/**
 * Initialize the id generator (seed for random)
 */
void init_id_generator() {
	srand(getpid() + time(NULL) + __LINE__);
}

/**
 * Slice a char* in a char**, n sized
 * Don't forget to free str_array
 */
int strncut(char ***str_array, char *str, size_t n) {
	int z;
	int blocs_count;
	size_t str_size;

	str_size = strlen(str);
	blocs_count = str_size / n;

	if (str_size % n == 0) {
		*str_array = (char **) malloc(sizeof(char *) * (blocs_count));
	} else {
		*str_array = (char **) malloc(sizeof(char *) * (blocs_count + 1));
	}
	for (z = 0; z != blocs_count; z++) {
		*(*str_array + z) = (char *) malloc(n);
		strncpy(*(*str_array + z), str + (n * z), n);
		strcat(*(*str_array + z), "");// adds a terminating null byte
	}

	if (z * n < str_size) {
		*(*str_array + z) = (char *) malloc(n);
		strncpy(*(*str_array + z), str + (n * z), (str_size - (n * z) ));
		z++;
	}

	return z;
}

/**
 * To free a 2D array of char
 */
void free_str_array(char **str_array, int len) {
	int i;

	for (i = 0; i != len; i++) {
		free(str_array[i]);
	}

	free(str_array);
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

