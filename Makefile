CC=gcc
CFLAGS=-Wall


FILES_UTILS=src/utils/utils.c
FILESH_UTILS=src/utils/utils.h

FILES_SHELL=src/shell/shell.c src/shell/commands.c
FILESH_SHELL=src/shell/shell.h src/shell/commands.h

FILES=src/main.c
HEADERS=src/main.h

COMMANDS_FILES=src/bin/*

FILES += $(FILES_UTILS) $(FILES_SHELL)
HEADERS += $(FILESH_UTILS) $(FILESH_SHELL)

DIR=build


systemd: $(FILES) $(HEADERS) $(COMMANDS_FILES)
	$(CC) $(CFLAGS) $(FILES) -o systemd $(LIBS)

src/bin/%: src/src/%.c
	find src/src/ -name *.c -exec bash -c "gcc {} -o src/bin/\`basename {} .c\`" \;
	
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
	rm -rf src/bin/*

.PHONY: doc
doc:
	rm -rf doc/html doc/latex
# 	doxygen doc/.conf
	doxygen doc/Doxyfile