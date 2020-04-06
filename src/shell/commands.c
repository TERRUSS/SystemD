
#include "commands.h"

Command *commands = NULL; // hashtable of all avaliables commands

int execute(char ** argv, int argc) {
	Command *c;
		//find command if its exists
	HASH_FIND_STR( commands, argv[0], c);
	if (c){

		#ifdef LOG_SHELL
		printf("exectuting command #%d\n", c->id);
		#endif

		return c->function(++argv);
	}
	else {
		printf("unknown command\n");
		return 1;
	}
}

int prim_quit(char ** param) {
		// free the commands hashmap
	Command *c, *tmp;
	HASH_ITER(hh, commands, c, tmp) {
		HASH_DEL(commands, c);
		free(c);
	}

	return -1;
}

int prim_ls(char ** param) {
	printf("Want to list ?\n");
	return 0;
}


	// fuctions hashmap
void initCommandsHashmap() {

	Command *c;
	char *coreCommands[] = { "quit", "exit", "ls", NULL };
	int *corePrimitives[] = { &prim_quit, &prim_quit, &prim_ls, NULL };

	for (int i = 0; coreCommands[i]; ++i) {
		c = (Command *)malloc(sizeof *c);
		c->name = coreCommands[i];
		c->id = i;
		c->function = corePrimitives[i];
		HASH_ADD_KEYPTR( hh, commands, c->name, strlen(c->name), c );
	}
}