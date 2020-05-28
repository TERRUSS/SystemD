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

unsigned int  get_pwd_id(){
	//void * cur = malloc(sizeof(unsigned int));
	unsigned int cur;
	char tmp[100];
	sscanf(getenv("SYSD_CURDIR"), "%u %*s", &cur, tmp);

	return cur;
}

int main(int argc, char const *argv[]) {

	initFS();

	char ** files_list = NULL;
	files_list = handleArgs(argc, argv);

	printf("Creating files :\n");

	struct inode cur_dir = get_inode_by_id(get_pwd_id());

	for (int i = 0; i < argc-1; i++) {
		printf("%s	", files_list[i]);

		create_emptyfile(&cur_dir, files_list[i], REGULAR_FILE);
	}

	printf("\n");

	return 0;
}