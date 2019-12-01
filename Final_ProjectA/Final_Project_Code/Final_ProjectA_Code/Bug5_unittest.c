/* -----------------------------------------------------------------------
 * alasagae - Final Project
 * BUG5 unittest: In the scoreFor function, the discardCount is being used for the deck count.
 *
 *
 *  Bug5_unittest: Bug5_unittest.c dominion.o rngs.o interface.o
 *      gcc -o Bug5_unittest -g  Bug5_unittest.c interface.o dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include "interface.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "rngs.h"

//SCOREFOR FUNCTION
    //Gain card: 
    //added card for [whoseTurn] current player:
    // toFlag = 0 : add to discard
    // toFlag = 1 : add to deck
    // toFlag = 2 : add to hand

int main() {
    int i, j, m;
    int seed = 1000;
    int p, r, handCount = 0;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;

    //Initialize to 2 player game and clear game state
    memset(&G, 23, sizeof(struct gameState));   // clear the game state 
    r = initializeGame(2, k, seed, &G);         // initialize a new game 2 player
    G.handCount[p] = handCount;                 // set the number of cards on hand
    //memcpy(G.hand[p], copper, sizeof(int) * handCount); // set all the cards to copper

    //Fill hand and discard and deckcount with cards
    G.deckCount[p] = 10;    

    //Fill deck count of player and opponent (10 cards each)
        for(i = 0; i < G.numPlayers; i++)
        {
            G.deckCount[i] = 10;
            //printf("Deck count = %d\n", G.deckCount[i]);
            int k =  G.deckCount[i];
            for(j = 0; j < G.deckCount[i]; j++)
            {
                if(j == 0)
                {
                    G.deck[i][j] = 3;   
                }
                else if (j == 1)
                {
                    G.deck[i][j] = 3;
                }
                else if (j == 2)
                {
                     G.deck[i][j] = 3;      //6 VP * 3 == 18 VP
                }
                else
                {
                     G.deck[i][j] = 1;      //Fill with Estate = 1 VP * 7 = 7VP
                }
            }
        }

        //TOTAL points for player and opponent deck == 25 VP
  
    //Fill discardCount cards:
    gainCard(1, &G, 0, p);  //1 VP
    gainCard(2, &G, 0, p);  //3 VP
    gainCard(3, &G, 0, p);  //6 VP
    //TOTAL in player discardCount = 10 VP

    //TOTAL player = 25VP + 10VP = 35VP
    //TOTAL opponent = 25VP (none in discard)

//TEST1: ABLE TO EXECUTE SCOREFOR FUNCTION AND RETURN TOTAL POINTS
        //int scoreFor (int player, struct gameState *state) 
 printf("---------------------------------------\n");
        printf("TEST 1: CHECK SCOREFOR CURRENT PLAYER\n");
        if(scoreFor(p, &G) == 35)
        {
            printf("    PASS: Player total score is 35\n");
        }
        else
        {
            printf("    FAIL: ScoreFor function does not return accurate score\n");
        }


 printf("---------------------------------------\n");
    printf("PRINT SCORES FOR 2 PLAYERS: \n");
    printScores(&G);

    printf("\nAll tests passed!\n");

    return 0;
}
