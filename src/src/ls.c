#include <stdio.h>
#include <stdlib.h>


char * handleArgs(int argc, char const *argv[]) {
	if (argc == 1) {
		char * path = malloc( sizeof(char) * 1);
		path[0] = '.';

		return path;
	}
	else if(argc == 2)
		return argv[1];
	else {
		exit(-1);
	}
}


int main(int argc, char const *argv[]) {

	char * path = NULL;

	path = handleArgs(argc, argv);

	printf("Wanna list %s ?\n", path);

	return 0;
}