CFLAGS= -Wall -g -fprofile-arcs -ftest-coverage -fpic -coverage -lm -std=c99

default: all

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

unittest1: unittest1.c dominion.o rngs.o interface.o
	gcc -o unittest1 -g  unittest1.c interface.o dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o interface.o
	gcc -o unittest2 -g  unittest2.c interface.o dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o interface.o
	gcc -o unittest3 -g  unittest3.c interface.o dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o interface.o
	gcc -o unittest4 -g  unittest4.c interface.o dominion.o rngs.o $(CFLAGS)

unittest5: unittest5.c dominion.o rngs.o interface.o
	gcc -o unittest5 -g  unittest5.c interface.o dominion.o rngs.o $(CFLAGS)

all: unittest1 unittest2 unittest3 unittest4 unittest5

runalltests: all
	./unittest1
	./unittest2
	./unittest3
	./unittest4
	./unittest5
	gcov -f -b dominion.c >> unittestresults.out

clean:
	rm -f *.o unittest1 unittest1.exe unittest2 unittest2.exe unittest3 unittest3.exe unittest4 unittest4.exe unittest5 unittest5.exe *.gcov *.gcda *.gcno *.so *.out