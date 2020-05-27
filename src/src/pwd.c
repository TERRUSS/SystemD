#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char * getPath() {
	return getenv("SYSD_CURDIR");
}


int main(int argc, char const *argv[]) {

	char * str_path = NULL;

	str_path = getPath();

	struct inode cur;
	memcpy(&cur, str_path, sizeof(str_path));

	printf("%s\n", get_filename_for_inode(&cur));

	return 0;
}