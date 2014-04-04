ABADDON = abaddon
FOLDERS = bin dbg
CC = gcc
LIB_FLAGS = -pthread
DEBUG_FLAGS = -g -O0 -Wall -Wextra -ansi-c --pedantic-errors -o dbg/abaddon_debug
SERVER_FLAGS = -O2 -o bin/abaddon

clear:
	rm -rf $(FOLDERS)

abaddon:
	mkdir -p bin
	$(CC) $(SERVER_FLAGS) $(LIB_FLAGS) *.c
	bin/abaddon $(PORT)

cgdb_debug:
	debug
	cgdb dbg/abaddon $(PORT)

valgrind_debug:
	debug
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes \
			 --num-callers=20 --track-fds=yes --track-origins=yes -v \
			 --read-var-info=yes dbg/abaddon $(PORT) 2> dbg/valgrind_err.txt

debug:
	mkdir -p dbg
	$(CC) $(DEBUG_FLAGS) $(LIB_FLAGS) *.c
