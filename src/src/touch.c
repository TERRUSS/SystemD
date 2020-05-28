#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char * handleArgs(int argc, char const *argv[]) {
	if (argc >= 2) {
		return ++argv;
	}
	else {
		printf("Missing argument, check the man page for more informations.\n");
		exit(-1);
	}
}

struct inode * get_pwd_id(){
	//void * cur = malloc(sizeof(unsigned int));
	struct inode * cur;
	char tmp[100];
	sscanf(getenv("SYSD_CURDIR"), "%p %*s", &cur, tmp);

	return cur;
}

int main(int argc, char const *argv[]) {

	char ** files_list = NULL;
	files_list = handleArgs(argc, argv);

	printf("Creating files :\n");

	struct inode * cur_dir = get_pwd_id();

	for (int i = 0; i < argc-1; i++) {
		printf("%s", files_list[i]);

		create_emptyfile(cur_dir, files_list[i], REGULAR_FILE);
	}

	printf("\n");

	return 0;
}