#ifndef FILEIO_H
#define FILEIO_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef LINE_SEPARATOR
# if defined(__WIN32) || defined(__WIN32__)
#  define LINE_SEPARATOR "\r\n"
#  define LINE_SEPARATOR_SIZE 2
# else
#  define LINE_SEPARATOR "\n"
#  define LINE_SEPARATOR_SIZE 1
# endif
#endif

char *rd(char *filename);
int cp(char *file_to_copy, char *file_dest);
int wr(char *filename, char *content);

#endif

