#include "fs/fs.h"

int test_create_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";


	struct inode i = create_inode(t, m, user, NULL);

	return 0;
}

int test_print_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";


	struct inode i = create_inode(t, m, user, NULL);
	print_inode(&i);

	return 0;
}

int test_write_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";
	struct inode i;

	i = create_inode(t, m, user, NULL);
	if (write_inode(&i) == 0)
		printf("Test success\n");
	else
		printf("Test failure\n");

	return 0;
}

int test_create_bloc() {
	struct bloc b;

	b = create_bloc("hello_world.c", "#include<stdio.h>\nint main(){printf(\"HelloWorld\n\");return 0;}");

	return 0;
}

int test_write_bloc() {
	struct bloc b;

	b = create_bloc("hello_world.c", "#include<stdio.h>\nint main(){printf(\"HelloWorld\n\");return 0;}");
	if (write_bloc(&b) == 0)
		printf("Test success\n");
	else
		printf("Test failure\n");

	return 0;
}

int test_print_disk() {
	print_disk();

	return 0;
}

int test_create_regularfile() {
	struct inode i;

	i = create_regularfile("toto", "");

	return 0;
}

int test_get_bloc_id() {
	struct inode i;

	if (get_bloc_id("toto") == -1) {
		printf("Test failure\n");
	} else {
		printf("Test success\n");
	}

	if (get_bloc_id("gqrgqratag") != -1) {
		printf("Test failure\n");
	} else {
		printf("Test success\n");
	}

	return 0;
}

int main() {

	/*
	test_create_inode();
	test_print_inode();
	test_write_inode();
	test_create_bloc();
	test_write_bloc();
	*/


	test_create_regularfile();
	test_get_bloc_id();
	test_print_disk();

	return 0;
}

