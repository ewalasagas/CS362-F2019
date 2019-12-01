CFLAGS= -Wall -g -fprofile-arcs -ftest-coverage -fpic -coverage -lm -std=c99

default: all

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

Bug5_unittest: Bug5_unittest.c dominion.o rngs.o interface.o
	gcc -o Bug5_unittest -g  Bug5_unittest.c interface.o dominion.o rngs.o $(CFLAGS)

Bug6_unittest: Bug6_unittest.c dominion.o rngs.o interface.o
	gcc -o Bug6_unittest -g  Bug6_unittest.c interface.o dominion.o rngs.o $(CFLAGS)

Bug7_unittest: Bug7_unittest.c dominion.o rngs.o interface.o
	gcc -o Bug7_unittest -g  Bug7_unittest.c interface.o dominion.o rngs.o $(CFLAGS)

Bug8_unittest: Bug8_unittest.c dominion.o rngs.o interface.o
	gcc -o Bug8_unittest -g  Bug8_unittest.c interface.o dominion.o rngs.o $(CFLAGS)

all: playdom player Bug5_unittest Bug6_unittest Bug7_unittest Bug8_unittest

runalltests: all
	./Bug5_unittest
	./Bug6_unittest
	./Bug7_unittest
	./Bug8_unittest
	gcov -f -b dominion.c >> testresults.out

clean:
	rm -f *.o playdom.exe playdom player player.exe Bug5_unittest Bug5_unittest.exe Bug6_unittest Bug6_unittest.exe Bug7_unittest Bug7_unittest.exe Bug8_unittest Bug8_unittest.exe *.gcov *.gcda *.gcno *.so *.out