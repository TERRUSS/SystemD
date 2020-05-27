#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char * handleArgs(int argc, char const *argv[]) {
	if (argc >= 2) {
		return ++argv;
	}
	else {
		exit(-1);
	}
}

struct inode * get_wd(){
	void * cur = malloc(sizeof(struct inode *));
	memcpy(cur, getenv("SYSD_CURDIR"), sizeof(getenv("SYSD_CURDIR")));

	return cur;
}

int main(int argc, char const *argv[]) {

	char ** files_list = NULL;
	files_list = handleArgs(argc, argv);

	printf("Creating files :\n");

	struct inode * cur_dir = get_wd();

	for (int i = 0; i < argc-1; i++) {
		printf("%s", files_list[i]);

		create_emptyfile(cur_dir, files_list[i], REGULAR_FILE);
	}

	printf("\n");

	return 0;
}