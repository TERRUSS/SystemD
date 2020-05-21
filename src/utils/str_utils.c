#include "utils/str_utils.h"

/**
 * Slice a char* in a char**, n sized
 * Don't forget to free str_array
 */
int strncut(char ***str_array, char *str, size_t n) {
	int z;
	int blocs_count;
	size_t str_size;

	str_size = strlen(str);
	blocs_count = str_size / n;

	if (str_size % n == 0) {
		*str_array = (char **) malloc(sizeof(char *) * (blocs_count));
	} else {
		*str_array = (char **) malloc(sizeof(char *) * (blocs_count + 1));
	}
	for (z = 0; z != blocs_count; z++) {
		*(*str_array + z) = (char *) malloc(n);
		strncpy(*(*str_array + z), str + (n * z), n - 1);
		strcat(*(*str_array + z), "\0");// adds a terminating null byte
	}

	if (z * n < str_size) {
		*(*str_array + z) = (char *) malloc(n);
		strncpy(*(*str_array + z), str + (n * z), (str_size - (n * z)));
		z++;
	}

	return z;
}


/**
 * To free a 2D array of char
 */
void free_str_array(char **str_array, int len) {
	int i;

	for (i = 0; i != len; i++) {
		free(str_array[i]);
	}

	free(str_array);
}

/**
 * Counts the number of occurences in a char*
 */
int ocr(char *str, char c) {
	int o;
	int i;

	printf(">%s\n", str);
	if (str == NULL) return 0;
	if (strcmp(str, "") == 0) return 0;

	i = 0;
	o = 0;
	while (str[i] != '\0') {
		if (str[i] == c) {
			o++;
		}
		i++;
	}

	return o;
}

/*
 * Returns the index of the first occurence of a character
 * in a string
 */
int get_index(char *str, char chr) {
    char *c;

    c = strchr(str, chr);
    return (int) (c - str);
}

/**
 * Parse inode ids in a bloc (directory)
 * Don't forget to free the array
 */
unsigned int *parse_ids(char *str) {
	int i;
	unsigned int *ids;
	int filecount;
	int o;

	filecount = ocr(str, ',');
	ids = (unsigned int *) malloc(sizeof(unsigned int) * filecount);

	for (i = 0; i != filecount; i++) {
		sscanf(str + o, "%u", ids + i);
		i++;
		o = get_index(str + o, ',');
	}

	return ids;
}

