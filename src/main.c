
/**
 * @file doxygen_c.h
 * @author My Self
 * @date 9 Sep 2012
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

/*! \file main.c
 * \brief SystemD main
 * \author H.HENROTTE, M.VINCENT, D.DANG, P.REPAIN, O.BENABEN
 * \version 1.0.0
 * \date 17 Ma*
i 2020
  *	Implémentation d'un système de fichiers en C (Projet scolaire)
 *
 */

#include "main.h"

int DEBUG = 0;


/**
	\fn int read(int fd,char *buf,size_t count)
    \brief Read bytes from a file descriptor.
    \param fd The descriptor to read from.
    \param buf The buffer to read into.
    \param count The number of bytes to read.
*/

/**
 * \fn int main (int argc, char const *argv[])
 * \brief Fonction main, Entrée du programme.
 *
 * \param argc int : nombre d'arguments
 * \param argv char * : liste des arguments
 * \return 0
 */
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

		res = execute(sd_argc, sd_argv);

		free(sd_argv);

	} while ( res != -1 );

	return 0;
}


/**
 * @brief Example showing how to document a function with Doxygen.
 *
 * Description of what the function does. This part may refer to the parameters
 * of the function, like @p param1 or @p param2. A word of code can also be
 * inserted like @c this which is equivalent to <tt>this</tt> and can be useful
 * to say that the function returns a @c void or an @c int. If you want to have
 * more than one word in typewriter font, then just use @<tt@>.
 * We can also include text verbatim,
 * @verbatim like this@endverbatim
 * Sometimes it is also convenient to include an example of usage:
 * @code
 * BoxStruct *out = Box_The_Function_Name(param1, param2);
 * printf("something...\n");
 * @endcode
 * Or,
 * @code{.py}
 * pyval = python_func(arg1, arg2)
 * print pyval
 * @endcode
 * when the language is not the one used in the current source file (but
 * <b>be careful</b> as this may be supported only by recent versions
 * of Doxygen). By the way, <b>this is how you write bold text</b> or,
 * if it is just one word, then you can just do @b this.
 * @param param1 Description of the first parameter of the function.
 * @param param2 The second one, which follows @p param1.
 * @return Describe what the function returns.
 * @see Box_The_Second_Function
 * @see Box_The_Last_One
 * @see http://website/
 * @note Something to note.
 * @warning Warning.
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