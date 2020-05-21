#include "fs/inode.h"

const mode_t DEFAULT_PERMISSIONS = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
const char ROOT[USERNAME_COUNT] = "root";
const mode_t ROOT_PERMISSIONS = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;

char g_username[USERNAME_COUNT];

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
 * Deletes an inode
 */
int delete_inode(struct inode *i) {
    if (i->id == DELETED) {
        return 0;
    } else {
        i->id = DELETED;
        return 1;
    }
}

/**
 * Initialize the id generator (seed for random)
 * TODO put in rand_utils/c
 */
void init_id_generator() {
	srand(getpid() + time(NULL) + __LINE__);
}

