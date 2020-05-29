#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

int main(int argc, char const *argv[]) {

	unsigned int blocs, inodes;
	size_t bytes;

	initFS();
	
	disk_free(&blocs, &inodes, &bytes);
	printf("blocs available %lu\n", blocs);
	printf("inodes available %lu\n", inodes);
	printf("bytes available %lu\n", bytes);

	printf("\n	See `diskimg` for a detailed look of the file system (related)\n");
	
	return 0;
}