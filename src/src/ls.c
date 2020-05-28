#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char * handleArgs(int argc, char const *argv[]) {
	if (argc == 1) {
		char * path = malloc( sizeof(char) * 1);
		path[0] = '.';

		return path;
	}
	else if(argc == 2)
		return argv[1];
	else {
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

	char * path = NULL;

	path = handleArgs(argc, argv);

	printf("Wanna list %s ?\n", path);

	int filecount;
	char ** files;
	struct inode * wd = get_pwd_id();

	files = list_files(wd, &filecount);
	print_str_array(files, filecount);
	free_str_array(files, filecount);

	return 0;
}