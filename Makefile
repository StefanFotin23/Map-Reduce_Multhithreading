# APD - Tema 1

build:
	gcc -o tema1 tema1.c -Wall -lpthread -lm

build_debug:
	gcc -o tema1 tema1.c -lpthread -Wall -Werror -lm -O0 -g3 -DDEBUG

clean:
	rm -rf tema1