#include "fileio/fileio.h"
#include "fs/fs.h"

int test_new_inode() {
	enum filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";


	struct inode i = new_inode(t, m, user, NULL);
	(void) i;

	printf("test_new_inode() successful\n");
	return EXIT_SUCCESS;
}

int test_print_inode() {
	enum filetype t = REGULAR_FILE;
	mode_t m = S_IRWXU;
	char user[10] = "Paul";


	struct inode i = new_inode(t, m, user, NULL);
	print_inode(&i);

	return EXIT_SUCCESS;
}

int test_write_inode() {
	enum filetype t = REGULAR_FILE;
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
	struct file f;

	clean_disk();
	g_working_directory = create_disk();

	f = create_regularfile(&g_working_directory, "FILENAME", "TRUC", O_RDONLY);
	strcpy(f.inode.user_name, "Pauli");
	if (update_inode(&(f.inode)) == EXIT_FAILURE) {
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

	str_array = NULL;
	len = strncut(&str_array, str, 2);
	if (len != 7) {
		perror("test_strncut() failed");
		return EXIT_FAILURE;
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
	create_regularfile(&g_working_directory, filename, content, O_RDONLY);
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
	struct file f;
	unsigned int blocs, inodes;
	size_t bytes;

	clean_disk();
	g_working_directory = create_disk();
	f = create_emptyfile(&g_working_directory, filename, REGULAR_FILE);
	content = rd("README.md");
	if (content == NULL) {
		perror("test_iwrite() failed");
		return EXIT_FAILURE;
	}

	iwrite(&f, content, 620);
	iwrite(&f, content, 120);
	disk_free(&blocs, &inodes, &bytes);
	if (blocs != 1 && inodes != 0) {
		perror("test_remove_file() failed");
		return EXIT_FAILURE;
	}
	free(content);

	printf("test_iwrite() successful\n");
	return EXIT_SUCCESS;
}

int test_get_filename_for_inode() {
	char filename[FILENAME_COUNT] = "FILENAME";
	char content[] = "TRUC";
	char *rst;
	struct file f;

	clean_disk();
	g_working_directory = create_disk();
	f = create_regularfile(&g_working_directory, filename, content, O_RDONLY);
	rst = get_filename_for_inode(&(f.inode));
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

int test_disk_free() {
	unsigned int blocs, inodes;
	size_t bytes;

	clean_disk();

	// write inode
	g_working_directory = create_disk();

	// check number of inodes == 0
	disk_free(&blocs, &inodes, &bytes);
	if (blocs != 0 && inodes != 0 && bytes != 0) {
		perror("test_disk_free() failed");
		return EXIT_FAILURE;
	}

	// delete dir
	create_directory(&g_working_directory, "dir");
	remove_empty_directory(&g_working_directory, "dir");

	// check number of inodes == 1
	disk_free(&blocs, &inodes, &bytes);
	if (blocs != 1 && inodes != 1) {
		perror("test_disk_free() failed");
		return 0;
	}

	printf("test_disk_free() successful\n");

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
	int filecount;
	char **files;

	clean_disk();
	g_working_directory = create_disk();
	dir = create_directory(&g_working_directory, "home");

	if (get_filecount(&g_working_directory) != 1) {
		perror("test_create_directory() failed");
		return EXIT_FAILURE;
	}
	if (get_filecount(&dir) != 2) {
		perror("test_create_directory() failed");
		return EXIT_FAILURE;
	}
	files = list_files(&dir, &filecount);
	printf("files[0] %s\n", files[0]);
	print_str_array(files, filecount);
	free_str_array(files, filecount);
	print_disk();

	printf("test_create_directory() successful\n");

	return EXIT_SUCCESS;
}

int test_create_emptyfile() {
	struct file f;

	clean_disk();
	g_working_directory = create_disk();
	f = create_emptyfile(&g_working_directory, "hello.py", REGULAR_FILE);

	if (f.inode.bloc_count != 1) {
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

	create_regularfile(&g_working_directory, "hey.txt", "OwO", O_RDONLY);
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
	char *content;
	char buf[121];
	char filename[FILENAME_COUNT] = "FILENAME";
	size_t n = 120;
	struct file f;

	clean_disk();
	g_working_directory = create_disk();
	content = rd("README.md");
	f = create_regularfile(&g_working_directory, filename, content, O_RDONLY);
	free(content);
	iread(&f, buf, n);

	if (buf == NULL) {
		perror("test_iread() failure");
		return EXIT_FAILURE;
	}

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
	create_regularfile(&g_working_directory, "FILENAME", "TRUC", O_RDONLY);
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
	struct file f1, f2;

	clean_disk();
	g_working_directory = create_disk();

	f1 = create_regularfile(&g_working_directory, "Bidsouf", "Yahoo", O_RDONLY);
	f2 = iopen(&g_working_directory, "Bidsouf", O_RDONLY);
	if (!inode_equals(f1.inode, f2.inode)) {
		perror("test_iopen() failed");
		return EXIT_FAILURE;
	}
	f1 = iopen(&g_working_directory, "Croute", O_RDONLY);
	if (!f1.inode.id == DELETED) {
		perror("test_iopen() failed");
		return EXIT_FAILURE;
	}

	printf("test_iopen() successful\n");

	return EXIT_SUCCESS;
}

int test_remove_empty_directory() {

	clean_disk();
	g_working_directory = create_disk();

	create_regularfile(&g_working_directory, "FILENAME", "TRUC", O_RDONLY);
	create_directory(&g_working_directory, "home");

	if (remove_empty_directory(&g_working_directory, "FILENAME") != EXIT_FAILURE
			&& get_filecount(&g_working_directory) != 2) {
		perror("test_remove_empty_directory() failed");
		return EXIT_FAILURE;
	}
	if (remove_empty_directory(&g_working_directory, "home") != EXIT_SUCCESS && get_filecount(&g_working_directory) != 1) {
		perror("test_remove_empty_directory() failed");
		return EXIT_FAILURE;
	}

	create_directory(&g_working_directory, "usr");
	create_regularfile(&g_working_directory, "user_file.sh", "echo 'je suis une loutre'", O_RDONLY);
	if (remove_empty_directory(&g_working_directory, "usr") != EXIT_FAILURE && get_filecount(&g_working_directory) != 3) {
		perror("test_remove_empty_directory() failed");
		return EXIT_FAILURE;
	}

	printf("test_remove_empty_directory() successful\n");
	return EXIT_SUCCESS;
}

int test_strjoin() {
	char str[] = "";
	int int_array[] = {12, 434, 121, 13};

	strjoin(str, int_array, 4, ',');
	if (strcmp(str, "12,434,121,13,") != 0) {
		fprintf(stderr, "Unexpected %s\n", str);
		return EXIT_FAILURE;
	}

	printf("test_strjoin() successful\n");
	return EXIT_SUCCESS;
}

int test_remove_file() {
	unsigned int blocs, inodes;
	size_t bytes;


	clean_disk();
	g_working_directory = create_disk();
	create_regularfile(&g_working_directory, "FILENAME", "TRUC", O_RDONLY);
	create_directory(&g_working_directory, "home");

	if (remove_file(&g_working_directory, "home", REGULAR_FILE) != EXIT_FAILURE) {
		perror("test_remove_file() failed");
		return EXIT_FAILURE;
	}

	disk_free(&blocs, &inodes, &bytes);
	if (blocs != 0 && inodes != 0 && bytes != 0) {
		perror("test_remove_file() failed");
		return EXIT_FAILURE;
	}

	if (remove_file(&g_working_directory, "FILENAME", REGULAR_FILE) != EXIT_SUCCESS) {
		perror("test_remove_file() failed");
		return EXIT_FAILURE;
	}
	disk_free(&blocs, &inodes, &bytes);
	if (blocs != 1 && inodes != 1) {
		perror("test_remove_file() failed");
		return EXIT_FAILURE;
	}

	printf("test_remove_file() successful\n");
	return EXIT_SUCCESS;
}

int test_move_file() {
	struct inode to;

	clean_disk();
	g_working_directory = create_disk();
	create_regularfile(&g_working_directory, "FILENAME", "TRUC", O_RDONLY);
	to = create_directory(&g_working_directory, "home");

	move_file(&g_working_directory, "FILENAME", &to);

	if (get_filecount(&g_working_directory) != 1) {
		perror("test_move_file() failed");
		return EXIT_FAILURE;
	}
	if (get_filecount(&to) != 3) {
		perror("test_move_file() failed");
		return EXIT_FAILURE;
	}

	printf("test_move_file() successful\n");

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

	test_strncut();
	test_get_filename_for_inode();
	test_list_files();
	test_update_inode();
	test_iread();
	test_strsplt();
	test_get_inodes();
	test_iopen();
	test_remove_empty_directory();
	test_strjoin();
	test_disk_free();
	test_remove_file();
	test_iwrite();
	test_move_file();

	return EXIT_SUCCESS;
}

