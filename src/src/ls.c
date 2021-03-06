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

int main(int argc, char const *argv[]) {

	char * path = NULL;

	path = handleArgs(argc, argv);

	int filecount;
	char ** files;
	struct inode wd = get_inode_by_id(get_pwd_id());

	files = list_files(&wd, &filecount);
	for (int i = 0; i < filecount; i++){
		printf("%s	", files[i]);
		free(files[i]);
	}
	if (filecount)
		printf("\n");
	free(files);

	return 0;
}