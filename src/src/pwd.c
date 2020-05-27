#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

unsigned int  get_pwd_id(){
	//void * cur = malloc(sizeof(unsigned int));
	unsigned int cur;
	char tmp[100];
	sscanf(getenv("SYSD_CURDIR"), "%u %*s", &cur, tmp);

	return cur;
}

int main(int argc, char const *argv[]) {
	unsigned int cur_dir = get_pwd_id();

	printf("%s\n", get_filename_for_inodeID(cur_dir));

	return 0;
}