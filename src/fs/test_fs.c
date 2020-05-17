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

	i = create_file("toto", REGULAR_FILE, "");

	return 0;
}

/*
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
*/

int test_update() {
	srand(getpid()+time(NULL)+__LINE__);
	struct bloc b;
	clean_disk();
	create_disk();
	b = create_bloc("hello_world.c", "#include<stdio.h>\nint main(){printf(\"HelloWorld\n\");return 0;}");
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";
	struct inode i;

	i = create_inode(t, m, user, NULL);
	write_inode(&i);
	//write inodes to disk
	//print disk
	print_disk();
	puts("");
	//update inode or bloc
	strcpy(i.user_name, "hello_world.cpp");
	strcpy(i.group_name, "hello_wezfz");
	update_inode(&i);
	//print disk
	puts("");
	print_disk();

	return 1;
}

int main() {

	/*
	test_create_inode();
	test_print_inode();
	test_write_inode();
	test_create_bloc();
	test_write_bloc();
	*/


	/*
	test_create_regularfile();
	test_get_bloc_id();
	test_print_disk();
	*/
	test_update();

	return 0;
}

