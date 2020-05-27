#include "shell.h"

char ** prompt( int * argc, int cmd_status, struct inode * pwd ) {
	char * input = 0;
	char ** argv = 0;

	if(cmd_status == 0 ) {
		printf("\033[1;35m┌─[\033[1;36muser\033[0;35m@\033[1;36mSYSTEMD\033[1;35m]─[%s]\n└──╼ \033[0;35m$\033[0m ", get_filename_for_inode(pwd));
	}
	else {
		printf("\033[1;35m┌─[✗]─[\033[1;36muser\033[0;35m@\033[1;36mSYSTEMD\033[1;35m]─[%s]\n└──╼ \033[0;35m$\033[0m ",  get_filename_for_inode(pwd));
	}

	input = readInput();
	argv = parseInput(input, argc);

	free(input);

	return argv;
}

char* readInput() {
	int CHUNK = 64;
	char* input = NULL;
	char tempbuf[CHUNK];
	size_t inputlen = 0, templen = 0;

	do {
		fgets(tempbuf, CHUNK, stdin);
		templen = strlen(tempbuf);
		input = realloc(input, inputlen+templen+1);
		strcpy(input+inputlen, tempbuf);
		inputlen += templen;
	} while (templen==CHUNK-1 && tempbuf[CHUNK-2]!='\n');

	input[strlen(input)-1] = 0; //remove last '\n' from the input
	return input;
}

char ** parseInput( char * line, int * argc ) {

	char *ptr = strtok(line, " ");
	char ** argv = 0;
	*argc = 0;

	while(ptr != NULL) {

		argv = realloc(argv, (*argc+1) * sizeof(char *));
		argv[*argc] = malloc(sizeof(char) * strlen(ptr));
		strcpy(argv[*argc], ptr);

		*argc+=1;
		ptr = strtok(NULL, " ");
	}

	//argv must end on NULL ptr
	argv = realloc(argv, (*argc+1) * sizeof(char *));
	argv[*argc] = NULL;

	return argv;
}


void clear() {
	if (!DEBUG)
		cls();

	printf("\033[1;36m");
	FILE* fp;
	char c;
	fp = fopen(BANNER_SRC, "r");
	if ( fp == NULL ) {
		printf( "! Banner file not found, (%s)\n", BANNER_SRC);
	}

	while((c = getc(fp)) != EOF)
		printf("%c", c);
	fclose(fp);
	printf("\n      \033[1;35m=== File Manager System of the future ===\033[0m\n\n");

	return;
}