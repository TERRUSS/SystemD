#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "../utils/globals.h"

typedef struct disk disk;
struct disk {
	FILE * file; // disc file

	inode root; // root inode
};

typedef struct inode inode;
struct inode {
	
};