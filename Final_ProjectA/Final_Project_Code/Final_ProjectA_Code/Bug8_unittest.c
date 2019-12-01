/* -----------------------------------------------------------------------
 * alasagae - Assignment #3
 * PROJECT ASSIGNMENT PART A:  BUG 08
 *
 * BUG DESCRIPTION: The number of bonus coins from actions does not
 *           appear to be recorded correctly in cardEffect. 
 *
 *
 * Include the following lines in your makefile (if not already included):
 *
 * Bug8_unittest.c: Bug8_unittest.c dominion.o rngs.o
 *      gcc -o Bug8_unittest -g  Bug8_unittest.c interface.o dominion.o rngs.o $(CFLAGS)
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

/*****************************BUG LIST #8*********************
*
*   BUG#8:  The number of bonus coins from actions does not appear to be 
*           recorded correctly in cardEffect. 
*
*   CHANGES: 
*       - In function "UpdateCoins()" removed state->coins = 0 (keeps resetting)
*       - In function "baron_ref()"  -- added "bonus" argument
*           - REMOVED: state->coins += 4;
*           - ADDED: bonus = 4;     updateCoins(currentPlayer, state, bonus);      
*       - In function "minion_ref()" -- added "bonus" argument
*           - REMOVED: (if Choice1) state->coins = state->coins + 2;
*           - ADDED: bonus = 2;      updateCoins(currentPlayer, state, bonus);
*       - In function "tribute_ref()" -- added "bonus" argument
*           - REMOVED: (Bottom - if card revealed is treasure card) state->coins += 2;
*           - ADDED:  bonus = 2;    updateCoins(currentPlayer, state, bonus;
*       - In function "Embargo" case -- added "bonus" argument
*           - REMOVED: (Begining of function) state->coins = state->coins + 2;
*            - ADDED: bonus = 2;    updateCoins(currentPlayer, state, bonus);
*       - In function "Feast" case is OK because it doesn't use coins
*       - In function "Cutpurse " case is OK because it uses "updateCoins(currentPlayer, state, 2);" 
*
**************************************************************/

int main() {
    int i, j;
    int seed = 1000;
    int p, r, handCount = 0;

    int k[10] = {adventurer, council_room, tribute, gardens, minion, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    //initialize game state and clear 
    memset(&G, 23, sizeof(struct gameState));   // clear the game state 
    r = initializeGame(2, k, seed, &G);         // initialize a new game with 2 plays so it's easier to capture cards
    G.handCount[p] = handCount;                 // set the number of cards on hand

//int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus) 


//TEST 1: TEST CARDEFFECT AND UPDATECOINS FUNCTION IN BARON - CHOICE1 = 1, BONUS = 2
    printf("---------------------------------------\n");
    printf("TEST 1: TEST CARDEFFECT AND UPDATECOINS FUNCTION IN BARON\n");
    G.coins = 5;    //initialize coins to 5

    //Fill hand with Baron and Estate card to discard 
    addCardToHand(p, 1, &G);
    addCardToHand(p, 15, &G);

    //Call cardEffect with Baron, where bonus = +4, when choice1 = 1 (discard Estate)
    cardEffect(15, 1, 0, 0, &G, 0, 4); 

    //Total coins 5 + 4 = 9
    if(G.coins == 9)
    {
        printf("    PASS: CardEffect of Baron gives +4 Bonus when Estate is discarded\n");
    }
    else
    {
        printf("    FAIL: CardEffect of Baron does not give accurate amount\n");
    }

//TEST 2: TEST CARDEFFECT AND UPDATECOINS FUNCTION IN MINION - CHOICE1 = 1, HANDPOS = 1, BONUS = 2
    //int minion_ref(struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos, int bonus)
    printf("---------------------------------------\n");
    printf("TEST 2: TEST CARDEFFECT AND UPDATECOINS FUNCTION IN MINION\n");
    G.coins = 5;    //initialize coins to 5

    //Add Minion to hand to test choice 1 of refactored function
    addCardToHand(p, 17, &G);

    //Call cardEffect with Minion, where bonus = +2, when choice1 = 1
     cardEffect(17, 1, 0, 0, &G, 1, 2);    

    //Total coins 5 + 2 = 7
    if(G.coins == 7)
    {
        printf("    PASS: CardEffect of Minion gives +2 Bonus\n");
    }
    else
    {
        printf("    FAIL: CardEffect of Minion does not give accurate amount\n");
    }

//TEST 3: TEST CARDEFFECT AND UPDATECOINS FUNCTION IN TRIBUTE - IF TREASURE CARD EXISTS IN HAND +2
    //CALL USING CARDEFFECT ONLY - TRIBUTE #19
    printf("---------------------------------------\n");
    printf("TEST 3: TEST CARDEFFECT AND UPDATECOINS FUNCTION IN TRIBUTE\n");
    int tributeRevealedCards[1];                //tribute cards array 
    G.coins = 5;    //initialize coins to 5

        for(i = 1; i < G.numPlayers; i++)
        {
            //printf("Player %d: deck size %d\n", i, G.deckCount[i]);     //deck count of player
            int k =  G.deckCount[i];
            for(j = 0; j < G.deckCount[i]; j++)
            {
                //treasure card -- gold #6
                if(j == k-2)
                {
                    G.deck[i][j] = 6;
                    tributeRevealedCards[0] = G.deck[i][j];
                }
                //action card -- village #14 
                else if(j == k-1)
                {
                    G.deck[i][j] = 14;
                    tributeRevealedCards[1] = G.deck[i][j];
                }
                else
                {
                     G.deck[i][j] = 1;      //Fill with Estate to check
                }
            }
        }    

    //Call cardEffect with tribute - if card is treasure, +2 coins
    cardEffect(19, 0, 0, 0, &G, 0, 2);    

    //Total coins 5 + 2 = 7
    if(G.coins == 7)
    {
        printf("    PASS: CardEffect of Tribute gives +2 Bonus when 1 treasure cards revealed\n");
    }
    else
    {
        printf("    FAIL: CardEffect of Tribute does not give accurate amount\n");
    }

//TEST 5: TEST EMBARGO FOR UPDATECOINS FUNCTION -- EMBARGO #22
        //CALL USING CARDEFFECT ONLY 
    printf("---------------------------------------\n");
    printf("TEST 4: TEST CARDEFFECT AND UPDATECOINS FUNCTION IN EMBARGO\n");
    G.coins = 0;    //initialize coins to 0
    cardEffect(22, 0, 0, 0, &G, 0, 2);    
    //Total coins 0 + 2 = 2
    if(G.coins == 2)
    {
        printf("    PASS: CardEffect of Embargo gives +2 Bonus when played\n");
    }
    else
    {
        printf("    FAIL: CardEffect of Embargo does not give accurate amount\n");
    }


    printf("\nAll tests passed!\n");

    return 0;
}
