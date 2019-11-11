CFLAGS = -Wall -g -fprofile-arcs -ftest-coverage -fpic -coverage -lm -std=c99

testme: testme.c 
	gcc -o testme -g  testme.c $(CFLAGS)

all: testme runtest

clean:
	rm -f *.o testme.exe testme *.gcov *.gcda *.gcno *.so *.out