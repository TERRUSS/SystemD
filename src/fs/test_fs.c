#include "fileio/fileio.h"
#include "fs/fs.h"

int test_new_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";


	struct inode i = new_inode(t, m, user, NULL);

	return 0;
}

int test_print_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";


	struct inode i = new_inode(t, m, user, NULL);
	print_inode(&i);

	return 0;
}

int test_write_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";
	struct inode i;

	i = new_inode(t, m, user, NULL);
	if (write_inode(&i) == 0)
		printf("Test success\n");
	else
		printf("Test failure\n");

	return 0;
}

int test_new_bloc() {
	struct bloc b;

	b = new_bloc("hello_world.c", "#include<stdio.h>\nint main(){printf(\"HelloWorld\n\");return 0;}");

	return 0;
}

int test_write_bloc() {
	struct bloc b;

	b = new_bloc("hello_world.c", "#include<stdio.h>\nint main(){printf(\"HelloWorld\n\");return 0;}");
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

int test_update() {
	struct bloc b;
	clean_disk();
	create_disk();
	b = new_bloc("hello_world.c", "#include<stdio.h>\nint main(){printf(\"HelloWorld\n\");return 0;}");
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";
	struct inode i;

	i = new_inode(t, m, user, NULL);
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


int test_strncut() {
	char **str_array;
	char str[] = "Hello world !";
	int len;
	int i;

	str_array = NULL;
	len = strncut(&str_array, str, 2);
	for (i = 0; i != len; i++) {
		printf("[%d] %lu %s\n", i, strlen(str_array[i]), str_array[i]);
	}
	free_str_array(str_array, len);

	return 1;
}

int test_create_regularfile() {
	char filename[FILENAME_COUNT] = "FILENAME";
	char *content;

	clean_disk();
	create_disk();
	content = rd("README.md");
	if (content == NULL) {
		perror("Test failed");
		return 0;
	} else {
		create_regularfile(filename, content);
		print_disk();
		free(content);
	}

	return 1;
}

int test_get_inode_blocs() {
	int z;
	char filename[FILENAME_COUNT] = "FILENAME";
	char *content;
	struct inode i;
	struct bloc *blocs;

	clean_disk();
	create_disk();
	content = rd("README.md");
	if (content == NULL) {
		perror("Test failed");
		return 0;
	} else {
		i = create_regularfile(filename, content);
		blocs = get_inode_blocs(&i);
		for (z = 0; z != i.bloc_count; z++) {
			print_bloc(blocs + z);
		}
		free(blocs);

		free(content);
	}

	return 1;
}

int test_iwrite() {
	char filename[FILENAME_COUNT] = "FILENAME";
	char *content;
	struct inode i;

	clean_disk();
	create_disk();
	i = create_emptyfile(filename, REGULAR_FILE, "a");
	content = rd("README.md");
	if (content == NULL) {
		perror("Test failed");
		return 0;
	} else {
		print_disk();
		iwrite(&i, content);
		print_disk();
		free(content);
	}

	return 1;
}

int test_add_inode_to_inode() {

	return 1;
}

int main() {

	init_id_generator();
	/*
	test_new_inode();
	test_print_inode();
	test_write_inode();
	test_new_bloc();
	test_write_bloc();
	*/


	/*
	test_create_regularfile();
	test_get_bloc_id();
	test_print_disk();
	*/

	//test_update();
	//test_strncut();
	/*test_create_regularfile();*/
	/*test_get_inode_blocs();*/
	test_iwrite();

	return 0;
}

