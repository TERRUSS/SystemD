#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int number(char const * str) {
	int res;
	sscanf(str, "%d", &res);
	printf("waiting %d seconds\n", res);
	return res;
}

int handleArgs(int argc, char const *argv[]) {
	if(argc==2)
		return number(argv[1]);
	else {
		exit(-1);
	}
}



int main(int argc, char const *argv[]) {
	sleep(handleArgs(argc, argv));
	printf("tx 4 waitin\n");

	return 0;
}
