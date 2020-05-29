#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

int main(int argc, char const *argv[]) {
	unsigned int cur_dir = get_pwd_id();

	char * fn = get_filename_for_inodeID(cur_dir);
	printf("%s\n", fn);
	free(fn);

	return 0;
}