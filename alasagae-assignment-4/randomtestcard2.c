/* -----------------------------------------------------------------------
 * alasagae - Assignment #4
 * RANDOM TEST 2:    MINION RANDOM TESTER
 *
 * TO COMPILE, RUN "MAKE ALL RUNALLTESTS"
 * RUN "CAT RAMDONTESTRESULTS.OUT" TO SEE COVERAGE
 * -----------------------------------------------------------------------
 */

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

// RULE: MINION function gives +1 ACTION 
//       CHOOSE 1:
//          A: +2 coins
//          B: Discard your hand, +4 cards, and each other player with at LEAST
//              5 cards in hand discards their hand and draws 4 cards from your deck

int choice()
{
    int num;
    num = rand() % 3;
    return num;
}

int random_card()
{
    int num;
    num = rand() % 25;
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

    //int minion_ref(struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos)

//Initialize game with 2 players
    r = initializeGame(2, k, seed, &G);         // initialize a new game 2 player
    G.handCount[p] = handCount;                 // set the number of cards on hand

printf("minion_ref function test with 2 players only:\n");
//RANDOM TEST FOR INPUTS/PARAMETERS OF MINION 
    for(i = 0; i < n; i++)
    {
        //put minion card in hand
        addCardToHand(p, random_card(), &G);   //player now has 1 card 
        addCardToHand(p, random_card(), &G);    //Add cards to hand to test discard hand
        int rand_num_pos = choice();
        int rand_num_choice2 = choice();
        printf("Test input: %d\n", rand_num_pos);
        minion_ref(&G, rand_num_pos, rand_num_choice2,  p, 1);
    }
    printf("\n");

//INITIALIZE WITH 4 PLAYERS
printf("Initialize for game for 4 players:\n");
int players = 4;
//INITIALIZE GAME FOR 2+ players
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(players, k, seed, &G);         // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand

//RANDOM TEST FOR INPUTS/PARAMETERS OF MINION 
    for(i = 0; i < n; i++)
    {
        addCardToHand(p, random_card(), &G);   //player now has 1 card 
        addCardToHand(p, random_card(), &G);    //Add cards to hand to test discard hand
        int rand_num_pos = choice();
        int rand_num_choice2 = choice();
        printf("Test input: %d\n", rand_num_pos);
        minion_ref(&G, rand_num_pos, rand_num_choice2,  p, 1);
    }
    printf("\n");

    printf("\nAll tests passed!\n");
    return 0;
}
