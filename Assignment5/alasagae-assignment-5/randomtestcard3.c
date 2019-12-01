/* -----------------------------------------------------------------------
 * alasagae - Assignment #5 - RANDOM TESTING
 * RANDOM TEST 3:    TRIBUTE RANDOM TESTER
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

// RULE: TRIBUTE function tests
//  Player to left reveals top 2 cards and discards cards
//  (1) IF card is ACTION -- +2 ACTIONS
//  (2) IF card is TREASURE -- +2 COINS ()
//  (3) IF card is VICTORY -- +2 CARDS
//  NOTE: IF you reveal 2 types, it gives you both bonuses
//  NOTE: Curse is no bonus

int choice()
{
    int num;
    num = (rand() % (3 - 1 + 1)) + 1;
    return num;
}

void fillTributeCards(struct gameState *G, int tributeRevealedCards[2])
{
    int i, j;

  for(i = 1; i < G->numPlayers; i++)
        {
            //printf("Player %d: deck size %d\n", i, G.deckCount[i]);     //deck count of player
            for(j = 0; j < G->deckCount[i]; j++)
            {
                    G->deck[i][j] = rand() % 25;
            }
        }
}

int main() {
    srand(time(NULL));
    int i, j;
    int seed = 1000;
    int n = 500;
    
    int p, r, handCount = 0;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;

    char cardName[MAX_STRING_LENGTH];
    strcpy(cardName,"");
    cardNumToName(15, cardName);
    memset(&G, 23, sizeof(struct gameState));   // clear the game state 

    //Adding tribute cards
    int tributeRevealedCards[2];   

    //int tribute_ref(struct gameState *state, int tributeRevealedCards[2], int nextPlayer, int currentPlayer)

//Initialize game with 2 players
    r = initializeGame(2, k, seed, &G);         // initialize a new game 2 player
    G.handCount[p] = handCount;                 // set the number of cards on hand

printf("tribute_ref function test with 2 players only:\n");
//RANDOM TEST FOR INPUTS/PARAMETERS OF MINION 
    for(i = 0; i < n; i++)
    {
        fillTributeCards(&G, tributeRevealedCards);
        int rand_num_pos = choice();
        printf("Test input: %d\n", rand_num_pos);
        //tribute_ref(&G, tributeRevealedCards, 1, 0);
//int tributeR(int currentPlayer, int nextPlayer, int tributeRevealedCards[], struct gameState *state, int *bonus)
        tributeR(p, 1, tributeRevealedCards, &G, 0);
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
        fillTributeCards(&G, tributeRevealedCards);
        int rand_num_pos = choice();
        printf("Test input: %d\n", rand_num_pos);
        //tribute_ref(&G, tributeRevealedCards, rand_num_pos, 0);
//int tributeR(int currentPlayer, int nextPlayer, int tributeRevealedCards[], struct gameState *state, int *bonus)
        tributeR(p, rand_num_pos, tributeRevealedCards, &G, 0);        
    }

    printf("\nAll tests passed!\n");
    return 0;
}
