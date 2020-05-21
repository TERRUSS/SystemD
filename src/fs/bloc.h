#ifndef BLOC_H
#define BLOC_H

#include "fs/inode.h"

#define BLOC_SIZE (512)
#define FILENAME_COUNT (15)

struct bloc {
	unsigned int id;

	char filename[FILENAME_COUNT];

	char content[BLOC_SIZE];
};

int add_bloc(struct inode *i, struct bloc *b);
struct bloc new_bloc(const char *filename, const char *content);
void delete_bloc(struct bloc *b);
void print_bloc(struct bloc *b);
struct bloc empty_bloc();

#endif

