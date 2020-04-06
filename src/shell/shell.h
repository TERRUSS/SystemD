#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BANNER_SRC "./src/banner.ascii"

char* readInput();
char ** parseInput( char * line, int * argc );
void clear();
char ** prompt( int * argc );