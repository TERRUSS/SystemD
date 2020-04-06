#include "main.h"

//#define LOG_SHELL

int main(/*int argc, char const *argv[]*/) {

	initSystemD();

	char ** argv = 0;
	int argc = 0;

	int res = 0;

	clear();

	do {
		free(argv);

		argv = prompt(&argc);

		#ifdef LOG_SHELL
		printf("[SD LOG] %s : %d parameters\n", argv[0], argc -1);
		for (int i = 0; i < argc; ++i) {
			printf("%d %s\n", i, argv[i]);
		}
		#endif

		res = execute(argv, argc);

	} while ( res != -1 );

		//quit
	free(argv);

	return 0;
}


void initSystemD(){
	initCommandsHashmap();
}