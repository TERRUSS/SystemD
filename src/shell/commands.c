
#include "commands.h"

int execute(int argc, char ** argv) {
	
	if(strcmp(argv[0], "exit") == 0){
		return 254;
	}

	pid_t pid = -1;
	int status = 0;
	char * path = NULL;

	if (DEBUG)
		printf("Collecting command path & env\n");

	path = getexecpath(path, "./src/bin/", argv[0]);

	if (DEBUG)
		printf("Executing : %s\n", path);


	pid = fork();

	if (pid == 0) {
		// Execute binary form /root/src/bin
		if (execv(path, argv) == -1) {
			printf(" ✗ - sdsh : %s: command not found\n", argv[0]);
			if (DEBUG) {
				perror("Err");
			}
		}
		exit(404);
	} else if (pid < 0) {
		// Error forking
		status = 503;
		perror("sdsh fork err");
	} else {
		// Parent process
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	free(path);

	if (DEBUG)
		printf(" » %d\n", status);
	return status;
}



char * getexecpath (char * path, char * root, char * name) {
	path = malloc(sizeof(char) * (strlen(root)+strlen(name)));

	strcat(path, root);
	strcat(&path[strlen(root)], name);

	return path;
}