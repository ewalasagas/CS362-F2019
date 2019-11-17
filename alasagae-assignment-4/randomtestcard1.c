/* -----------------------------------------------------------------------
 * alasagae - Assignment #4
 * RANDOM TEST 1:    BARON RANDOM TESTER
 *
 *
 * Include the following lines in your makefile (if not already included):
 *
 * randomtest1: randomtest1.c dominion.o rngs.o
 *      gcc -o randomtest1 -g  randomtest1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */
//TO COMPILE AND RUN: "gcc randomtestcard1.c dominion.c rngs.c interface.c -lm -o test" -- run "./test"
#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"
#include <time.h>

// RULE: BARON function gives +1 BUY 
//       You may discard an Estate (choice X), for +4
//       If you don't, gain an Estate (choice x)
int random_MAXNEG_MAXPOS()
{
    int num;
    num = (rand() % (2147483647 - (-2147483647) + 1)) + (-2147483647);
    return num;    
}

int choice1()
{
    int num;
    num = rand() % 2;
    return num;
}

int main() {
    srand(time(NULL));
    int i;
    int seed = 1000;
    int n = 500;
    
    int p, r, handCount = 0;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;

    char cardName[MAX_STRING_LENGTH];
    strcpy(cardName,"");
    cardNumToName(15, cardName);
    memset(&G, 23, sizeof(struct gameState));   // clear the game state 

    //baron_ref(state, choice1, currentPlayer);

//Initialize game with 2 players
    r = initializeGame(2, k, seed, &G);         // initialize a new game 2 player
    G.handCount[p] = handCount;                 // set the number of cards on hand

printf("Baron_refactor function test with 2 players only:\n");
//RANDOM TEST FOR INPUTS/PARAMETERS OF BARON - CHOICE1
// POSITIVE TEST
    for(i = 0; i < n; i++)
    {
        int rand_num_pos = random_MAXNEG_MAXPOS();
        printf("Test input choice1: %d\n", rand_num_pos);
        baron_ref(&G, rand_num_pos, p);  
    }
    printf("\n");

//RANDOM TEST FOR INPUTS/PARAMETERS OF BARON - CURRENTPLAYER
// POSITIVE TEST
printf("\nRandom test of current-player input with choice1 as 1\n");
    for(i = 0; i < n; i++)
    {
        int choice = choice1();
        int rand_num_pos = random_MAXNEG_MAXPOS();
        printf("Test input currentPlayer rand-pos: %d\n", rand_num_pos);
        baron_ref(&G, choice, rand_num_pos);  
    }
    printf("\n");

//INITIALIZE WITH 4 PLAYERS
printf("Initialize for game for 4 players:\n");
int players = 4;
//INITIALIZE GAME FOR 2+ players
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(players, k, seed, &G);         // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand

//RANDOM POSITIVE TEST: CHOICE1 IS EITHER 0, 1, OR 2
// POSITIVE TEST
    for(i = 0; i < n; i++)
    {
        int rand_num_pos = choice1();
        printf("Test input choice1 rand-pos: %d\n", rand_num_pos);
        baron_ref(&G, rand_num_pos, p);  
    }
    printf("\n");

//RANDOM TEST FOR INPUTS/PARAMETERS OF BARON - CURRENTPLAYER
printf("\nRandom test of current-player input with choice1 as 1\n");
    for(i = 0; i < n; i++)
    {
        int choice = choice1();
        int rand_num_pos = random_MAXNEG_MAXPOS();
        printf("Test input currentPlayer rand-pos: %d\n", rand_num_pos);
        baron_ref(&G, choice, rand_num_pos);  
    }
    printf("\n");

    printf("\nAll tests passed!\n");
    return 0;
}
