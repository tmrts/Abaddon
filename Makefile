ABADDON = abaddon
FOLDERS = bin dbg
CC = gcc
LIB_FLAGS = -pthread
DEBUG_FLAGS = -g -O0 -Wall -Wextra -o dbg/abaddon_debug
FLAGS = -O2 -o bin/abaddon

clear:
	rm -rf $(FOLDERS)

cppcheck:
	mkdir -p dbg
	cppcheck *.c > dbg/cppcheck.txt
	
abaddon:
	mkdir -p bin
	$(CC) $(FLAGS) $(LIB_FLAGS) *.c
	bin/abaddon $(PORT)

cgdb_debug:
	mkdir -p dbg
	$(CC) $(DEBUG_FLAGS) $(LIB_FLAGS) *.c
	cgdb dbg/abaddon --args $(PORT)

valgrind_debug:
	mkdir -p dbg
	$(CC) $(DEBUG_FLAGS) $(LIB_FLAGS) *.c
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes \
			 --num-callers=20 --track-fds=yes --track-origins=yes -v \
			 --read-var-info=yes dbg/abaddon $(PORT) 2> dbg/valgrind_err.txt

