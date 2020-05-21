#ifndef STR_UTILS_H
#define STR_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int strncut(char ***str_array, char *str, size_t n);
int get_index(char *str, char chr);
int ocr(char *str, char c);
unsigned int *parse_ids(char *str);

#endif

