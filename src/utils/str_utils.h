#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char **init_str_array(unsigned int len, unsigned int size);
int get_index(char *str, char chr);
int ocr(char *str, char c);
int strncut(char ***str_array, char *str, size_t n);
unsigned int *parse_ids(char *str);
void free_str_array(char **str_array, int len);
void print_str_array(char **str_array, int len);

#endif

