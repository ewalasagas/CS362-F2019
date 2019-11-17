CFLAGS= -Wall -g -fprofile-arcs -ftest-coverage -fpic -coverage -lm -std=c99

default: all

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

randomtestcard1: randomtestcard1.c dominion.o rngs.o interface.o
	gcc -o randomtestcard1 -g  randomtestcard1.c interface.o dominion.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o rngs.o interface.o
	gcc -o randomtestcard2 -g  randomtestcard2.c interface.o dominion.o rngs.o $(CFLAGS)

randomtestcard3: randomtestcard3.c dominion.o rngs.o interface.o
	gcc -o randomtestcard3 -g  randomtestcard3.c interface.o dominion.o rngs.o $(CFLAGS)

all: randomtestcard1 randomtestcard2 randomtestcard3

runalltests: all
	./randomtestcard1
	./randomtestcard2
	./randomtestcard3
	gcov -f -b dominion.c >> randomtestresults.out

clean:
	rm -f *.o randomtestcard1 randomtestcard1.exe randomtestcard2 randomtestcard2.exe randomtestcard3 randomtestcard3.exe*.gcov *.gcda *.gcno *.so *.out