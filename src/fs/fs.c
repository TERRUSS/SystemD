#include "fs/fs.h"

/**
 * Returns an inode
 *
 * user mut not be NULL
 * group can be NULL
 *
 * on success : returns inode
 * on failure : returns NULL
 */
struct inode create_inode(filetype type, mode_t perms, unsigned char *user, unsigned char *group) {

	struct inode i;
	time_t t;

	srand(getpid()+time(NULL));
	i.id = rand();

	i.type = type;
	i.permissions = perms;

	i.user_name = user;
	i.group_name = group;

	time(NULL);
	i.created_at = localtime(&t);
	i.updated_at = localtime(&t);

	i.bloc_ids = NULL;

	return i;
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

	/* TODO put flag to indicate it's an inode */
	fwrite(i, sizeof(struct inode), 1, f);

	return fclose(f);
}

