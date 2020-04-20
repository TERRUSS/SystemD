#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/globals.h"

	// SHELL
#include "shell/shell.h"
#include "shell/commands.h"

void initSystemD();
void handleArgs(int argc, char const *argv[]);