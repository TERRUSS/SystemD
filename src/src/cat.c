#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char ** handleArgs(int argc, char const *argv[]) {
	if (argc == 2) {
		return ++argv;
	}
	else {
		printf("cat : wrong number of parameters.\n");
		printf("Try 'man cat' for more information.\n");
		exit(-1);
	}
}

int main(int argc, char const *argv[]) {

	initFS();

	char ** arg = NULL;
	arg = handleArgs(argc, argv);

	struct file f;
	struct inode cur_dir = get_inode_by_id(get_pwd_id());

	f = iopen(&cur_dir, arg[0], O_RDWR);

	char * buf;
	buf = malloc(sizeof(char) * (get_total_strlen(&f.inode) + 1));

	iread(&f, buf, get_total_strlen(&f.inode));
	printf("%s\n", buf);

	return 0;
}