#include "./str_utils.h"

/**
 * Slice a char* in a char**, in chunks n-sized
 *
 * note: don't forget to free str_array
 * returns: the length of the array of strings
 */
int strncut(char ***str_array, char *str, size_t n) {
	int z;
	int blocs_count;
	size_t str_size;

	str_size = strlen(str);
	blocs_count = str_size / n;

	if (str_size % n == 0) {
		*str_array = init_str_array(blocs_count, n);
	} else {
		*str_array = init_str_array(blocs_count + 1, n);
	}
	for (z = 0; z != blocs_count; z++) {
		strncpy(*(*str_array + z), str + (n * z), n - 1);
		strcat(*(*str_array + z), "\0");
	}

	if (z * n < str_size) {
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
 *
 * excptions: str NULL, str == ""
 * on failure: returns 0
 */
unsigned int ocr(char *str, char c) {
	int o;
	int i;

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

/*
 * Splits a string into ints, the integers being separated by sep
 *
 * note: don't forget to free the array of ints
 */

unsigned int strsplt(char *str_tosplit, int **ints, char sep) {
	int i;
	unsigned int filecount;
	int o;

	filecount = ocr(str_tosplit, ',');
	*ints = (int *) calloc(filecount, sizeof(int));
	o = 0;

	for (i = 0; i < filecount; i++) {
		sscanf(str_tosplit + sizeof(char)*o, "%d", *(ints) + i);
		//printf("file %d/%u : %d, o = %d \n", i, filecount, *(*(ints) + i), o);
		o += get_index(str_tosplit + o, sep)+1;
	}

	return filecount;
}

/*
 * Prints an array of string len-sized into the console
 */
void print_str_array(char **str_array, int len) {
	int z;

	if (str_array == NULL) return;

	for (z = 0; z != len; z++) {
		printf("\t%s", str_array[z]);
	}
	puts("");
}

/*
 * Initialize an array of strings, the array being len-sized
 * and the strings size-sized xD
 */
char **init_str_array(unsigned int len, unsigned int size) {
	char **str_array;
	int z;

	str_array = (char **) malloc(len * sizeof(char *));

	for (z = 0; z != len; z++) {
		str_array[z] = (char *) calloc(size, sizeof(char));
	}

	return str_array;
}

/*
 * Join integers into a string, separated by sep
 */
int strjoin(char *str, int *int_array, int len, char sep) {
	int z;
	size_t offset;
	char tmp[10];
	int i;

	for (z = 0, offset = 0; z != len; z++, offset++) {
		sprintf(tmp, "%d", int_array[z]);
		strcat(str + offset, tmp);
		offset += strlen(tmp);
		str[offset] = sep;
		/* Add after loop */
		str[offset + 1] = '\0';
	}

	return EXIT_SUCCESS;
}



