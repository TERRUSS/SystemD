
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
#include "fileio/fileio.h"
#include "fs/fs.h"

int DEBUG = 0;

int main(int argc, char const *argv[]) {

	//--------
	
	struct stat buffer;   
	if (stat (DISK, &buffer) == 0)
		g_working_directory = get_inode_by_id(ROOT_ID);
	else
		g_working_directory = create_disk();
	ch_dir(ROOT_ID);

	if(DEBUG)
		printf("FS created : root @ %s", get_filename_for_inode(&g_working_directory));
	//---------

	handleArgs(argc, argv);

	char ** sd_argv = 0;
	int sd_argc = 0;

	int cmd_status = 0;

	clear();

	do {
		sd_argv = prompt(&sd_argc, cmd_status, &g_working_directory);

		if (DEBUG) {
			printf("[SD LOG] %s : %d parameters\n", sd_argv[0], sd_argc -1);
			for (int i = 0; i < sd_argc; ++i) {
				printf("%d %s\n", i, sd_argv[i]);
			}
		}

		if (sd_argc > 0)
			cmd_status = execute(sd_argc, sd_argv);

		for(int i = 0; i < sd_argc; i++){
			free(sd_argv[i]);
		}
		free(sd_argv);

		g_working_directory = get_inode_by_id(get_pwd_id());
		if (DEBUG)
			printf("[SD] current directory @%u (%s)\n", g_working_directory.id, getenv("SYSD_CURDIR"));
	} while ( cmd_status != 154 );

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