#ifndef BLOC_H
#define BLOC_H

#include "inode.h"

#define BLOC_SIZE (512)
#define FILENAME_COUNT (sizeof(char)*15)

struct bloc {
	unsigned int id;

	char filename[FILENAME_COUNT];

	char content[BLOC_SIZE];
};

int add_bloc(struct inode *i, struct bloc *b);
struct bloc empty_bloc();
struct bloc new_bloc(const char *filename, const char *content);
void print_bloc(struct bloc *b);

#endif

