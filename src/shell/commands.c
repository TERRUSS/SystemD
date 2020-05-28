
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
	char ** env = getEnv();

	if (DEBUG)
		printf("Executing : %s\n", path);


	pid = fork();

	if (pid == 0) {
		// Execute binary form /root/src/bin
		if (execve(path, argv, env) == -1) {
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


char ** getEnv(){
	char ** env = malloc(2 * sizeof( char * ));

	char* SYSD_CURDIR = "SYSD_CURDIR=";

	char * curdir = getenv("SYSD_CURDIR");

	if (DEBUG)
		printf("	%s : %s\n", SYSD_CURDIR, curdir);

	if (curdir) {
		env[0] = malloc( (sizeof(char) * strlen(SYSD_CURDIR)) + sizeof(curdir));
		strcat(env[0], SYSD_CURDIR);
		memcpy(&env[0][strlen(SYSD_CURDIR)], curdir, sizeof(curdir));
	}
	else {
		printf("ERROR : $SYSD_CURDIR not defined. Exiting...\n");
		exit(-1);
	}
	

	env[1] = NULL;

	if (DEBUG)
		printf("ENV\n	%s\n", env[0]);

	return env;
}