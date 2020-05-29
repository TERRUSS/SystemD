#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char ** handleArgs(int argc, char const *argv[]) {
	if (argc >= 2) {
		return ++argv;
	}
	else {
		exit(-1);
	}
}


int main(int argc, char const *argv[]) {

	initFS();

	char ** files_list = NULL;
	files_list = handleArgs(argc, argv);

	printf("Removing directories :\n");

	struct inode cur_dir = get_inode_by_id(get_pwd_id());

	for (int i = 0; i < argc-1; i++) {
		printf("%s	", files_list[i]);

		remove_empty_directory(&cur_dir, files_list[i]);
	}

	printf("\n");

	return 0;
}