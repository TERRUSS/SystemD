#include "fs.h"

disk * initFS() {

	int num;
	
	char * diskPath = NULL;

	do { // check if the disk is avaliables
		diskPath = getDiskPath();
	} while (!diskPath);

	return loadFS(diskPath);
}


char * getDiskPath() {
	FILE *fptr;

	char * diskPath;

	diskPath = malloc( strlen("disk.sysd") * char); //default disk is ./disk.sysd
	strcpy(diskPath, "disk.sysd")


	if ( checkDiskFile(diskPath) < 0 ) {	// no default disk, ask the user for what to do

		switch ( recoveryPrompt(diskPath) ) {
			case 0:
				return createDisk();
			case 1:
				if ( checkDiskFile(diskPath) )
					return diskPath;
				else 
					printf("No disk avaliable @ %s, check if the path is correct.\nBack to main menu.\n\n", diskPath);
					return NULL;
			case 3:
				return NULL; //haha go back brrrr
		}

	}

	return diskPath

}


int checkDiskFile(char * path) {
	fptr = fopen(path,"r");
	fclose(fptr); // just checking if the file exists

	if (fptr == NULL)
		return -1;
	else
		return 0;
}

int recoveryPrompt() {
	int choice = 0;

	printf("No default disk detected (@ ./disk.sysd). What do you want to do ?\n");

	while (!choice) {
		printf("1 > Create a default disk\n");
		printf("2 > Load an existing disk\n");
		printf("\n(1-2)>>> ");
		scanf(" %d", &choice);

		if (choice > 0 || choice <= 2) {
			printf("Wrong answer.\n");
		}
	}

	if (choice == 1) {
		return 0;
	}
	else {
		printf("Please enter the name of the disk :\n");
		printf("(leave empty to leave)>>> ");

		path = readInput();

		if (path[0] != '\n') {
			return 2;
		} else {
			return 3;
		}
	}

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