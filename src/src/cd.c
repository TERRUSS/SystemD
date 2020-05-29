#include <stdio.h>
#include <stdlib.h>
#include "../fs/fs.h"

char ** handleArgs(int argc, char const *argv[]) {
	if (argc == 2) {
		return ++argv;
	}
	if (argc == 1){
		return NULL;
	}	
	else {
		exit(-1);
	}
}

int main(int argc, char const *argv[]) {

	char ** arg = NULL;
	arg = handleArgs(argc, argv);

	if(arg == NULL)
		update_path(ROOT_ID);

	else {
		struct inode cur_dir = get_inode_by_id(get_pwd_id());
		struct inode target = get_inode_by_filename(&cur_dir, arg[0]);
		update_path(target.id);

		printf("chdir to %s (%u)\n", arg[0], target.id	);
	}

	return 0;
}