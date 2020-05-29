#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char ** handleArgs(int argc, char const *argv[]) {
	if (argc == 3) {
		return ++argv;
	}
	else {
		printf("write : wrong number of parameters.\n");
		printf("Try 'man write' for more information.\n");
		exit(-1);
	}
}

int main(int argc, char const *argv[]) {

	initFS();

	char ** arg = NULL;
	arg = handleArgs(argc, argv);

	struct file f;
	struct inode cur_dir = get_inode_by_id(get_pwd_id());

	printf("Writing \"%s\" in %s\n", arg[0], arg[1]);

	f = iopen(&cur_dir, arg[1], O_WRONLY);
	iwrite(&f, arg[0], strlen(arg[0]));

	return 0;
}