ABADDON = abaddon
FOLDERS = bin dbg
CC = gcc
LIB_FLAGS = -pthread
DEBUG_FLAGS = -g -O0 -o dbg/abaddon_dbg
EXTRA_FLAGS = -Wall -Wextra
FLAGS = -O2 -o bin/abaddon
VALGRING_FLAGS = --tool=memcheck --leak-check=yes --show-reachable=yes \
				 --num-callers=20 --track-fds=yes --track-origins=yes \ 
				 --read-var-info=yes

clear:
	rm -rf $(FOLDERS)

cppcheck:
	mkdir -p dbg
	cppcheck *.c > dbg/lint_log.txt
	
abaddon:
	mkdir -p bin
	$(CC) $(FLAGS) $(LIB_FLAGS) *.c
	bin/abaddon $(PORT) 2> bin/abaddon_log.txt

cgdb_debug:
	mkdir -p dbg
	$(CC) $(DEBUG_FLAGS) $(EXTRA_FLAGS) $(LIB_FLAGS) *.c > dbg/compile_log.txt
	cgdb dbg/abaddon_dbg --args $(PORT)

valgrind_debug:
	mkdir -p dbg
	$(CC) $(DEBUG_FLAGS) $(LIB_FLAGS) *.c
	valgrind $(VALGRIND_FLAGS) dbg/abaddon_dbg $(PORT) 2> dbg/valgrind_log.txt

