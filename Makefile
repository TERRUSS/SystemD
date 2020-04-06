CC=gcc
CFLAGS=-Wall


FILES_UTILS=src/utils/utils.c
FILESH_UTILS=src/utils/utils.h

FILES_SHELL=src/shell/shell.c src/shell/commands.c
FILESH_SHELL=src/shell/shell.h src/shell/commands.h

FILES=src/main.c
HEADERS=src/main.h


FILES += $(FILES_UTILS) $(FILES_SHELL)
HEADERS += $(FILESH_UTILS) $(FILESH_SHELL)

DIR=build


systemd: $(FILES) $(HEADERS)
	$(CC) $(CFLAGS) $(FILES) -o systemd $(LIBS)

tar: clean
	rm -rf $(DIR)
	mkdir $(DIR)
	cp exemple.c $(DIR)
	cp lib/graphics.c $(DIR)
	cp lib/graphics.h $(DIR)
	cp lib/couleur.h $(DIR)
	cp exo1.1.c $(DIR)
	cp Makefile $(DIR)
	cp lib/*.ttf $(DIR)
	tar cvf $(DIR).tar $(DIR)
	rm -rf $(DIR)

clean:
	rm -f *.o
	rm -f systemd
	rm -rf $(DIR)