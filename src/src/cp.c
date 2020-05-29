#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char ** handleArgs(int argc, char const *argv[]) {
	if (argc == 3) {
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

	printf("Cpying file :	");

	struct inode cur_dir = get_inode_by_id(get_pwd_id());

	printf("%s in %s\n", files_list[0], files_list[1]);

	copy_file(&cur_dir, files_list[0], files_list[1]);

	printf("\n");

	return 0;
}