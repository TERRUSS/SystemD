#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../utils/globals.h"
#include "../utils/utils.h"
#include "../fs/inode.h"
#include "../fs/fs.h"

#define BANNER_SRC "./src/banner.ascii"

char* readInput();
char ** parseInput( char * line, int * argc );
void clear();
char ** prompt( int * argc, int cmd_status, struct inode * pwd );