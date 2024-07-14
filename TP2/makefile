VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 -g
CC = gcc

all: clean valgrind-chanutron tp2

valgrind-chanutron: pruebas_chanutron
	valgrind $(VALGRIND_FLAGS) ./pruebas_chanutron

valgrind-alumno: pruebas_menu
	valgrind $(VALGRIND_FLAGS) ./pruebas_menu

valgrind-ejemplo: tp2
	valgrind $(VALGRIND_FLAGS) ./tp2
	
pruebas_chanutron: src/*.c pruebas_chanutron.c
	$(CC) $(CFLAGS) src/*.c pruebas_chanutron.c -o pruebas_chanutron

pruebas_menu: src/*.c pruebas_menu.c
	$(CC) $(CFLAGS) src/*.c pruebas_menu.c -o pruebas_menu

ejemplo: src/*.c tp2.c
	$(CC) $(CFLAGS) src/*.c tp2.c -o tp2

clean:
	rm -f pruebas_alumno pruebas_chanutron pruebas_menu tp2
