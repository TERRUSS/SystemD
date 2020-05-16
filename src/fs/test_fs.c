#include "fs/fs.h"

int test_create_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	unsigned char user[10] = "Paul";


	struct inode i = create_inode(t, m, user, NULL);

	return 0;
}


int test_print_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	unsigned char user[10] = "Paul";


	struct inode i = create_inode(t, m, user, NULL);
	print_inode(&i);

	return 0;
}

int main() {
	test_create_inode();
	test_print_inode();

	return 0;
}

