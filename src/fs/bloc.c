#include "fs/bloc.h"

/**
 * Adds a bloc id to an inode
 * TODO add return code
 * on success : returns 1
 * on failure : returns 0
 */
int add_bloc(struct inode *i, struct bloc *b) {
	if (i->bloc_count == BLOC_IDS_COUNT) {
		perror("Can't add anymore blocs to the inode !");
		return 0;
	} else {
		i->bloc_ids[i->bloc_count] = b->id;
		i->bloc_count++;
		return 1;
	}
}

/**
 * Returns a bloc
 *
 * filename must not be NULL (except for root)
 * content can be NULL
 */
struct bloc new_bloc(const char *filename, const char *content) {
	struct bloc b;

	memset(&b, 0, sizeof(struct bloc));
	b.id = rand();

	strncpy(b.filename, filename, FILENAME_COUNT);

	if (content == NULL) {
		strcpy(b.content, "");
	} else {
		strcpy(b.content, content);
	}

	return b;
}

struct bloc empty_bloc() {
	struct bloc b;
	memset(&b, 0, sizeof(struct bloc));
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

