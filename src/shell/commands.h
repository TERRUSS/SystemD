#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "../lib/uthash/uthash.h"
#pragma GCC diagnostic pop

typedef struct Command {
	char *name;
	int id;
	int (*function)(char **);
	UT_hash_handle hh; /* hashtable */
} Command;


void initCommandsHashmap();
int execute(char ** argv, int argc);
