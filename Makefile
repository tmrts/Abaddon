ABADDON = abaddon
FOLDERS = bin dbg
CC = gcc
LIB_FLAGS = -lpthread
DEBUG_FLAGS = -g -O0 -Wall -Wextra -ansi-c --pedantic-errors -o dbg/abaddon_debug
SERVER_FLAGS = -O2 -o bin/abaddon


clear:
	rm -rf $(FOLDERS)

abaddon:
	mkdir -p bin
	$(CC) $(LIB_FLAGS) $(SERVER_FLAGS) *.c
	bin/abaddon

cgdb_debug:
	debug
	cgdb dbg/abaddon

valgrind_debug:
	debug
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes \
			 --num-callers=20 --track-fds=yes --track-origins=yes -v \
			 --read-var-info=yes dbg/abaddon 2> dbg/valgrind_err.txt

debug:
	mkdir -p dbg
	$(CC) $(LIB_FLAGS) $(DEBUG_FLAGS) *.c
