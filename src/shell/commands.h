#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../utils/globals.h"
#include "../utils/utils.h"

int execute(int argc, char ** argv);
char * getexecpath (char * path, char * root, char * name);
char ** getEnv();