CC=gcc
CFLAGS=-Wall

FILES_UTILS=src/utils/utils.c
FILESH_UTILS=src/utils/utils.h
FILES_SHELL=src/shell/shell.c src/shell/commands.c
FILESH_SHELL=src/shell/shell.h src/shell/commands.h

FILES_FS=src/utils/str_utils.c src/fileio/fileio.c src/fs/inode.c src/fs/bloc.c src/fs/fs.c

FILES=src/main.c
HEADERS=src/main.h


FILES += $(FILES_UTILS) $(FILES_SHELL) $(FILES_FS)
HEADERS += $(FILESH_UTILS) $(FILESH_SHELL)

DIR=build

systemd: $(FILES) $(HEADERS) $(COMMANDS_FILES) commands
	$(CC) $(CFLAGS) $(FILES) -o systemd $(LIBS)

.PHONY: commands
commands:
	find src/src/ -name *.c -exec bash -c "gcc $(FILES_FS) {} -o src/bin/\`basename {} .c\`" \;

clean:
	rm -f *.o
	rm -f systemd
	rm -rf $(DIR)
	rm -rf src/bin/*

.PHONY: fs_test
fs_test:
	gcc -Isrc src/utils/str_utils.c src/fileio/fileio.c src/fs/inode.c src/fs/bloc.c src/fs/fs.c src/fs/test_fs.c 

.PHONY: clean_disk
clean_disk:
	rm rsc/disk