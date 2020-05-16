#include "fs/fs.h"

const int INODE_FLAG = 1;
const int BLOC_FLAG = 2;

/**
 * Returns an inode
 *
 * user mut not be NULL
 * group can be NULL
 *
 * on success : returns inode
 * on failure : returns NULL
 */
/*struct inode create_inode(filetype type, mode_t perms, unsigned char *user, unsigned char *group) {*/
struct inode create_inode(filetype type, mode_t perms, const char *user, const char *group) {

	struct inode i;
	time_t t;
	const char default_string[15] = "default";

	srand(getpid()+time(NULL));
	i.id = rand();

	i.type = type;
	i.permissions = perms;

	/*
	i.user_name = user;
	i.group_name = group;
	*/
	strcpy(i.user_name, user);
	if (group != NULL)
		strcpy(i.group_name, group);
	else
		strcpy(i.group_name, default_string);


	time(NULL);
	i.created_at = localtime(&t);
	i.updated_at = localtime(&t);

	/*i.bloc_ids = NULL;*/

	return i;
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
	printf("inode id:%d", i->id);
	printf(" filetype:%d", i->type);
	printf(" permissions:%d", i->permissions);
	printf(" user:%s", i->user_name);
	printf(" group:%s", i->group_name);
	assert(strftime(s, sizeof(s), "%c", i->created_at));
	assert(strftime(s2, sizeof(s2), "%c", i->updated_at));
	printf(" created at:%s", s);
	printf(" updated at:%s", s2);
	puts("");
	/*
	printf(" created at: %d-%02d-%02d %02d:%02d:%02d",
	i->created_at.tm_year + 1900, i->created_at.tm_mon + 1,
	i->created_at.tm_mday, i->created_at.tm_hour, i->created_at.tm_min,
	i->created_at.tm_sec);
	printf(" updated at: %d-%02d-%02d %02d:%02d:%02d\n",
	i->updated_at.tm_year + 1900, i->updated_at.tm_mon + 1,
	i->updated_at.tm_mday, i->updated_at.tm_hour, i->updated_at.tm_min,
	i->updated_at.tm_sec);
	*/
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

	srand(getpid()+time(NULL));
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
			printf("Bloc\n");
			fread(&b, sizeof(struct bloc), 1, f);
			print_bloc(&b);

		} else if (flag == INODE_FLAG) {
			printf("Inode\n");
			fread(&i, sizeof(struct inode), 1, f);
			print_inode(&i);

		} else {
			printf("What's this ?\n");
		}

	} while (size != 0);

	return fclose(f);
}

