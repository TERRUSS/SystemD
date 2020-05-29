#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char ** handleArgs(int argc, char const *argv[]) {
	if (argc == 3) {
		return ++argv;
	}
	else {
		printf("cp : wrong nomber of parameters.\n");
		printf("Try 'man cp' for more information.\n");
		exit(-1);
	}
}

int main(int argc, char const *argv[]) {
	printf("stp\n");

	initFS();

	char ** arg = NULL;
	arg = handleArgs(argc, argv);

	printf("Cpying file :	\n");

	struct inode cur_dir = get_inode_by_id(get_pwd_id());

	printf("%s in %s\n", arg[0], arg[1]);

	copy_file(&cur_dir, arg[0], arg[1]);

	return 0;
}