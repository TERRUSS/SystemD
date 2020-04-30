
/**
 * @file main.c
 * @authors H.HENROTTE, M.VINCENT, D.DANG, P.REPAIN, O.BENABEN
 * @date 9 17 Mai 2020
 * @brief SystemD main
 * 
 * Boucle principale :
 * 	Prompt et traitement de la commande
 *

 */

/**
 * @function main int
 * @brief Fonction d'entrée du programme
 *
 * @param argc int : nombre de paramètres du programme
 * @param argv char*[]: tableau des paramètres
 */

#include "main.h"

int DEBUG = 0;

int main(int argc, char const *argv[]) {

	handleArgs(argc, argv);

	char ** sd_argv = 0;
	int sd_argc = 0;

	int res = 0;

	clear();

	do {

		sd_argv = prompt(&sd_argc);

		if (DEBUG) {
			printf("[SD LOG] %s : %d parameters\n", sd_argv[0], sd_argc -1);
			for (int i = 0; i < sd_argc; ++i) {
				printf("%d %s\n", i, sd_argv[i]);
			}
		}

		if (sd_argc > 0)
			res = execute(sd_argc, sd_argv);

		free(sd_argv);

	} while ( res != -1 );

	return 0;
}


/**
 * @function void handleArgs
 * @brief Parsing des options du programes
 *
 * Appelée au lancement du programme par la fonction main
 * options :
 * 	> --debug
 *
 * @param argc int : nombre de paramètres du programme
 * @param argv char*[]: tableau des paramètres
 */
void handleArgs(int argc, char const *options[]) {
	if(argc==1) 
		return;
	else {
		for(int i=1 ; i < argc ; i++) {
				printf("\noptions[%d]: %s",i,options[i]); 
				printf("\n");

			if ( strcmp(options[i], "--debug") == 0 ) {
				DEBUG = 1;
				printf("DEBUG LOG ENABLED (%d)\n", DEBUG);
			}
		}
	}
	return; 
}