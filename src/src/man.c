#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAN_FOLDER "./src/man/"

char * handleArgs(int argc, char const *argv[]) {

	if(argc==2)
		return argv[1];
	else {
		exit(-1);
	}
}

char * getPath(char * command, char * path){

	int length = strlen(MAN_FOLDER) + strlen(command);

	path = malloc( sizeof(char)*length );

	strcat(path, MAN_FOLDER);
	strcat(&path[strlen(MAN_FOLDER)], command);

	return path;
}


int main(int argc, char const *argv[]) {


	FILE* fp;
	char c;
	
	char * path = NULL;
	char * command = handleArgs(argc, argv);
	path = getPath(command, path);

	fp = fopen(path, "r");

	if ( fp == NULL ) {
		printf( "! MAN page not found (%s)\n", command);
	}

	while((c = getc(fp)) != EOF)
		printf("%c", c);
	printf("\n");

	fclose(fp);
	free(path);

	return 0;
}