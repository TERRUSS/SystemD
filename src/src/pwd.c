#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

struct inode * get_pwd_id(){
	//void * cur = malloc(sizeof(unsigned int));
	struct inode * cur;
	char tmp[100];
	sscanf(getenv("SYSD_CURDIR"), "%p %*s", &cur, tmp);

	return cur;
}

int main(int argc, char const *argv[]) {
	struct inode * cur_dir = get_pwd_id();
	printf("ALLER LA %p\n", cur_dir);
	printf("%s\n", get_filename_for_inode(cur_dir));

	return 0;
}