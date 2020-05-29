
#include "commands.h"

int execute(int argc, char ** argv) {
	
	if(strcmp(argv[0], "exit") == 0){
		return 666;
	}

	pid_t pid = -1;
	int status = 0;
	char * path = NULL;

	if (DEBUG)
		printf("Collecting command path & env\n");

	path = getexecpath(path, "./src/bin/", argv[0]);

	if (DEBUG)
		printf("Executing : %s\n", path);


	key_t key = ftok("systemd",65);
	int shmid = shmget(key,1024,0666|IPC_CREAT);
	char *shared = (char*) shmat(shmid,(void*)0,0); 

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

		unsigned int wd = 0;
		sscanf(shared, "%u", &wd);
		if (wd > 0 && wd != g_working_directory.id){
			if (DEBUG)
				printf("changing dir to @%d", wd);
			ch_dir(wd);
		}
	}

	shmdt(shared);
	free(path);

	if (DEBUG)
		printf(" » %d\n", status);
	return status/255;
}



char * getexecpath (char * path, char * root, char * name) {
	int n = strlen(root)+strlen(name);
	path = malloc(sizeof(char) * n);

	//strcat(path, root);
	//strcat(&path[strlen(root)], name);

	memcpy(path, root, strlen(root));
	memcpy(&path[strlen(root)], name, strlen(name));
	path[n] = '\0'; //for an unknown reason it is needed if the command argc > 2...

	return path;
}