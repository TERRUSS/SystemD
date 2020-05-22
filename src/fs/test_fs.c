#include "fileio/fileio.h"
#include "fs/fs.h"

int test_new_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";


	struct inode i = new_inode(t, m, user, NULL);
	(void) i;

	printf("test_new_inode() successful\n");
	return EXIT_SUCCESS;
}

int test_print_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";


	struct inode i = new_inode(t, m, user, NULL);
	print_inode(&i);

	return EXIT_SUCCESS;
}

int test_write_inode() {
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";
	struct inode i;

	i = new_inode(t, m, user, NULL);
	if (write_inode(&i) != EXIT_SUCCESS)
		perror("test_write_inode() failed");
	printf("test_write_inode() success\n");

	return EXIT_SUCCESS;
}

int test_new_bloc() {
	struct bloc b;

	b = new_bloc("hello_world.c", "#include<stdio.h>\nint main(){printf(\"HelloWorld\n\");return 0;}");
	printf("test_new_bloc() successful\n");
	(void) b;

	return EXIT_SUCCESS;
}

int test_write_bloc() {
	struct bloc b;

	b = new_bloc("hello_world.c", "#include<stdio.h>\nint main(){printf(\"HelloWorld\n\");return 0;}");
	if (write_bloc(&b) != EXIT_SUCCESS)
		perror("test_write_bloc() failure");

	printf("test_write_bloc() successful\n");

	return EXIT_SUCCESS;
}

int test_print_disk() {
	print_disk();

	return 0;
}

int test_create_disk() {
	struct inode i;

	clean_disk();
	i = create_disk();
	(void) i;

	printf("test_create_disk() successful\n");
	return EXIT_SUCCESS;
}


int test_update_inode() {
	struct inode i;

	clean_disk();
	g_working_directory = create_disk();

	i = create_regularfile(&g_working_directory, "FILENAME", "TRUC");
	strcpy(i.user_name, "Pauli");
	if (update_inode(&i) == EXIT_FAILURE) {
		perror("test_update_inode() failed");
		return EXIT_FAILURE;
	}

	printf("test_update_inode() successful\n");
	return EXIT_SUCCESS;
}


int test_strncut() {
	char **str_array;
	char str[] = "Hello world !";
	int len;
	int i;

	str_array = NULL;
	len = strncut(&str_array, str, 2);
	if (len != 7) {
		perror("test_strncut() failed");
		return EXIT_FAILURE;
	}
	for (i = 0; i != len; i++) {
		printf("[%d] %lu %s\n", i, strlen(str_array[i]), str_array[i]);
	}
	free_str_array(str_array, len);

	printf("test_strncut() successful\n");
	return EXIT_SUCCESS;
}

int test_create_regularfile() {
	char filename[FILENAME_COUNT] = "FILENAME";
	char *content;

	clean_disk();
	g_working_directory = create_disk();

	content = rd("README.md");
	if (content == NULL) {
		perror("test_create_regularfile() failed");
		return EXIT_FAILURE;
	}
	create_regularfile(&g_working_directory, filename, content);
	if (get_filecount(&g_working_directory) != 1) {
		perror("test_create_regularfile() failed");
		return EXIT_FAILURE;
	}
	free(content);
	printf("test_create_regularfile() succesful\n");

	return EXIT_SUCCESS;
}

int test_get_inode_blocs() {
	clean_disk();
	create_disk();

	return 1;
}

int test_iwrite() {
	char filename[FILENAME_COUNT] = "FILENAME";
	char *content;
	struct inode i;

	clean_disk();
	g_working_directory = create_disk();
	i = create_emptyfile(&g_working_directory, filename, REGULAR_FILE, "a");
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

int test_get_filename_for_inode() {
	char filename[FILENAME_COUNT] = "FILENAME";
	char content[] = "TRUC";
	char *rst;
	struct inode i;

	clean_disk();
	g_working_directory = create_disk();
	i = create_regularfile(&g_working_directory, filename, content);
	rst = get_filename_for_inode(&i);
	if (rst == NULL) {
		perror("Ah !");
		return EXIT_FAILURE;
	}

	if (strncmp(rst, filename, FILENAME_COUNT) != 0) {
		perror("test_get_filename_for_inode() failed");
		return EXIT_FAILURE;
	}
	free(rst);

	printf("test_get_filename_for_inode() succesful\n");

	return 1;
}

int test_inode_count() {
	unsigned int available, deleted;
	filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";
	struct inode i, i2;

	clean_disk();

	// write inode
	i = create_disk();

	// check number of inodes == 1
	inode_count(&available, &deleted);
	if (available != 1 && deleted != 0) {
		printf("available %u deleted %u\n", available, deleted);
		perror("test_inode_count() failed");
		return 0;
	}

	// delete inode
	delete_inode(&i);

	// check number of inodes == 0
	inode_count(&available, &deleted);
	if (available != 0 && deleted != 1) {
		printf("available %u deleted %u\n", available, deleted);
		perror("test_inode_count() failed");
		return 0;
	}

	// write inode
	i2 = new_inode(t, m, user, NULL);
	write_inode(&i2);

	// check number of inodes == 1
	inode_count(&available, &deleted);
	if (available != 1 && deleted != 0) {
		printf("available %u deleted %u\n", available, deleted);
		perror("test_inode_count() failed");
		return 0;
	}

	printf("test_inode_count() successful\n");

	return 1;
}

int test_add_inode_to_inode() {
	struct inode i, i2;
	struct bloc b;

	clean_disk();
	create_disk();
	b = new_bloc("dir", "");
	i = new_inode(DIRECTORY, DEFAULT_PERMISSIONS, g_username, g_username);
	write_bloc(&b);
	add_bloc(&i, &b);
	write_inode(&i);

	// check filecount == 0
	if (get_filecount(&i) != 0) {
		fprintf(stderr, "test_add_inode_to_inode() failed\n");
		return EXIT_FAILURE;
	}

	// add inode/file to dir
	b = new_bloc("file.py", "print('Hello World')\\n");
	i2 = new_inode(REGULAR_FILE, DEFAULT_PERMISSIONS, g_username, g_username);
	write_inode(&i2);
	write_bloc(&b);
	add_bloc(&i2, &b);

	b = add_inode_to_inode(&i, &i2);
	update_bloc(&b);
	// check filecount == 1
	if (get_filecount(&i) != 1) {
		fprintf(stderr, "test_add_inode_to_inode() failed\n");
		return EXIT_FAILURE;
	}

	printf("test_add_inode_to_inode() successful\n");
	return EXIT_SUCCESS;
}

int test_create_directory() {
	struct inode dir;

	clean_disk();
	g_working_directory = create_disk();
	dir = create_directory(&g_working_directory, "home");

	if (get_filecount(&g_working_directory) != 1) {
		perror("test_create_directory() failed");
		return EXIT_FAILURE;
	}
	if (get_filecount(&dir) != 0) {
		perror("test_create_directory() failed");
		return EXIT_FAILURE;
	}

	printf("test_create_directory() successful\n");

	return EXIT_SUCCESS;
}

int test_create_emptyfile() {
	struct inode i;

	clean_disk();
	g_working_directory = create_disk();
	i = create_emptyfile(&g_working_directory, "hello.py", REGULAR_FILE, "a+");

	if (i.bloc_count != 1) {
		perror("test_create_emptyfile() failed");
		return EXIT_FAILURE;
	}
	if (get_filecount(&g_working_directory) != 1) {
		perror("test_create_emptyfile() failed");
		return EXIT_FAILURE;
	}

	printf("test_create_emptyfile() successful\n");
	return EXIT_SUCCESS;
}

int test_list_files() {
	int filecount;
	char **files;

	clean_disk();
	g_working_directory = create_disk();
	create_directory(&g_working_directory, "home");

	files = list_files(&g_working_directory, &filecount);
	print_str_array(files, filecount);
	free_str_array(files, filecount);

	create_regularfile(&g_working_directory, "hey.txt", "OwO");
	files = list_files(&g_working_directory, &filecount);
	print_str_array(files, filecount);
	free_str_array(files, filecount);

	printf("test_list_files() successful\n");
	return EXIT_SUCCESS;
}

int test_get_filecount() {
	clean_disk();
	g_working_directory = create_disk();
	if (get_filecount(&g_working_directory) != 0) {
		perror("test_get_filecount() failed");
		return EXIT_FAILURE;
	}

	printf("test_get_filecount() successful\n");
	return EXIT_SUCCESS;
}

int test_iread() {
	char filename[FILENAME_COUNT] = "FILENAME";
	char *content;
	struct inode i;
	char buf[121];
	size_t n = 120;

	clean_disk();
	g_working_directory = create_disk();
	content = rd("README.md");
	i = create_regularfile(&g_working_directory, filename, content);
	free(content);
	iread(&i, buf, n);

	if (buf == NULL) {
		perror("test_iread() failure");
		return EXIT_FAILURE;
	}
	printf("%s\n%lu\n%lu\n", buf, sizeof(buf), strlen(buf));

	printf("test_iread() successful\n");

	return EXIT_SUCCESS;
}

int test_strsplt() {
	int len;
	char c[] = "1231,23423,12,2,";
	int *ints;


	len = strsplt(c, &ints, ',');

	if (len != 4 && ints[0] != 1231 && ints[1] != 23423) {
		perror("test_strsplt() failed");
		return EXIT_FAILURE;
	}
	free(ints);

	printf("test_strsplt() successful\n");

	return EXIT_SUCCESS;
}

int test_get_inodes() {
	int len, z;
	struct inode *inodes;

	clean_disk();
	g_working_directory = create_disk();
	create_regularfile(&g_working_directory, "FILENAME", "TRUC");
	create_directory(&g_working_directory, "home");
	len = get_inodes(&g_working_directory, &inodes);

	if (len != 2) {
		perror("NON");
		for (z = 0; z != len; z++)
			print_inode(inodes + z);
		free(inodes);
		return EXIT_FAILURE;
	}

	free(inodes);
	printf("test_get_inodes() successful\n");
	return EXIT_SUCCESS;
}

int test_iopen() {
	struct inode i, j;

	clean_disk();
	g_working_directory = create_disk();

	i = create_regularfile(&g_working_directory, "Bidsouf", "Yahoo");
	j = iopen(&g_working_directory, "Bidsouf", "a");
	if (!inode_equals(i, j)) {
		perror("test_iopen() failed");
		return EXIT_FAILURE;
	}
	j = iopen(&g_working_directory, "Croute", "a");
	if (!j.id == DELETED) {
		perror("test_iopen() failed");
		return EXIT_FAILURE;
	}

	printf("test_iopen() successful\n");

	return EXIT_SUCCESS;
}

int test_remove_empty_directory() {

	clean_disk();
	g_working_directory = create_disk();

	create_regularfile(&g_working_directory, "FILENAME", "TRUC");
	create_directory(&g_working_directory, "home");
	print_disk();

	if (remove_empty_directory(&g_working_directory, "FILENAME") != EXIT_FAILURE) {
		perror("Wrong1");
		return EXIT_FAILURE;
	}
	if (remove_empty_directory(&g_working_directory, "home") != EXIT_SUCCESS) {
		perror("Wrong2");
		return EXIT_FAILURE;
	}

	create_directory(&g_working_directory, "usr");
	create_regularfile(&g_working_directory, "user_file.sh", "echo 'je suis une loutre'");
	if (remove_empty_directory(&g_working_directory, "usr") != EXIT_FAILURE) {
		perror("Wrong3");
		return EXIT_FAILURE;
	}
	print_disk();

	printf("test_remove_empty_directory() successful\n");
	return EXIT_SUCCESS;
}

int main() {

	init_id_generator();
	strcpy(g_username, "Paul");

	test_new_inode();
	test_print_inode();
	test_write_inode();
	test_new_bloc();
	test_write_bloc();

	test_create_disk();
	test_print_disk();
	test_get_filecount();
	test_add_inode_to_inode();
	test_create_regularfile();
	test_create_emptyfile();
	test_create_directory();
	test_inode_count();

	test_strncut();
	/*test_get_inode_blocs();*/
	/*test_iwrite();*/
	test_get_filename_for_inode();
	test_list_files();
	test_update_inode();
	test_iread();
	test_strsplt();
	test_get_inodes();
	test_iopen();
	test_remove_empty_directory();

	return EXIT_SUCCESS;
}

