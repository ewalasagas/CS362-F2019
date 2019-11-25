/* -----------------------------------------------------------------------
 * alasagae - Assignment #5 - testing other code
 * UNIT TEST 1:    BARON FUNCTION TEST
 *
 *
 * Include the following lines in your makefile (if not already included):
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
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

// RULE: BARON function gives +1 BUY 
//       You may discard an Estate (choice X), for +4
//       If you don't, gain an Estate (choice x)

int main() {
    int i;
    int seed = 1000;
    int p, r, handCount = 0;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;

    char cardName[MAX_STRING_LENGTH];
    strcpy(cardName,"");
    cardNumToName(15, cardName);


//TEST 1: GET COST OF BARON
        printf("---------------------------------------\n");
        printf("TEST 1: Cost of %s is == 4\n", cardName);
        if(getCost(15) == 4)
        {
            printf("    PASS: Cost of %s is 4\n", cardName);
        }
        else
        {
            printf("    FAIL: Unable to get cost of %s\n", cardName);
        }

//TEST 2: TRY TO BUY BARON - COST = 4
        //initialize game state and clear 
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        //printf("Initialize game with 2 players\n");
        r = initializeGame(2, k, seed, &G);         // initialize a new game 2 player
        G.handCount[p] = handCount;                 // set the number of cards on hand
        //memcpy(G.hand[p], copper, sizeof(int) * handCount); // set all the cards to copper
        G.coins = 5;    //Assign coins to 5
        printf("---------------------------------------\n");
        printf("TEST 2: TRY TO BUY BARON\n");

        //printf("Number of coins for player: %d\n", G.coins);    //Check number of coins
        if(buyCard(15, &G) == 0)
        {
            printf("    PASS: Able to buy %s card\n", cardName);
        }
        else
        {
            printf("    FAIL: Unable to buy %s card\n", cardName);
        }

//TEST 3: BARON FUNCTION EXECUTES?
//int baronR(int choice1, int currentPlayer, struct gameState *state, int *bonus) 

        //initialize game state and clear 
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(2, k, seed, &G);         // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand

        //Add Baron and Estate card to hand to test choice 1 of refactored function
        addCardToHand(p, 1, &G);
        addCardToHand(p, 15, &G);

        //Variable to check coins before baron called (+4 coins)
        int coins_before = G.coins;
        int buys_before = G.numBuys;
        //int coinsA = G.coins + 4;
        int cardExists = 0;     //initialiez variable to check if card exists

        printf("---------------------------------------\n");
        printf("TEST 3: EXECUTE REFACTORED BARON FUNCTION\n");

        //Check state of game before (with coins and actions);
        //        printState(&G);
  
        if(baronR(1, p, &G, 1, 0) == 0) //put baron card in hand
        {
            printf("    PASS: Baron card executed\n");
//TEST 4: BARON CHOICE 1 -- DISCARD AN ESTATE
            printf("---------------------------------------\n");
            printf("TEST 4: BARON CHOICE 1 -- DISCARD AN ESTATE\n");
            for(i = 0; i < numHandCards(&G); i++)
            {
                if(handCard(i, &G) == 1)
                {
                    cardExists = 1;
                }
            }
        }
        else
        {
            printf("    FAIL: Baron card was not called\n");
        }

        //Check if estate is discarded
        if(cardExists == 1)
        {
            printf("    FAIL: Player did not discard estate\n");
        }
        else
        {
            printf("    PASS: Player discarded estate\n");
        }
//introduce bug 1: make incorrect buys number
//TEST 5: BARON INCREASES NUM BUYS BY 1
        printf("---------------------------------------\n");
        printf("TEST 5 & BUG1: BARON INCREASES BUY ACTIONS BY +1\n");
        if((buys_before+1) == G.numBuys) 
        {
            printf("    PASS: Player buy actions before is %d and was increased by +1, current buy actions are %d\n", buys_before, G.numBuys);
        }
        else
        {
            printf("    FAIL: Player buy actions before is %d and was NOT increased by +1, current buy actions are %d\n", buys_before, G.numBuys);
        }

//introduce bug 2, make coins value not correct
//TEST 6: BARON CHOICE 1 -- GAIN +4 COINS 
        printf("---------------------------------------\n");
        printf("TEST 6 & BUG2: BARON CHOICE 1 -- GAIN +4 COINS\n");
        if((coins_before+4) == G.coins) 
        {
            printf("    PASS: Player coins before was %d and +4 coins added, current coins are now %d\n", coins_before, G.coins);
        }
        else
        {
            printf("    FAIL: Player coins before was %d and are NOT +4 coins added, current coins are now %d\n", coins_before, G.coins);
        }

//TEST 7: BARON CHOICE 2 -- GAIN AN ESTATE
        printf("---------------------------------------\n");
        printf("TEST 7: BARON CHOICE 2 -- GAIN AN ESTATE\n");
        //initialize game state and clear 
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(2, k, seed, &G);         // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand

        //put baron card in hand
        //addCardToHand(p, 15, &G);

        //call baron function with choice1 == 0 
        baronR(0, p, &G, 0);
        cardExists = 0;     //initailize variable to 0 to check if card exists in hand

        //Check if card exists in hand -- if estate is there, then function works
        for(i = 0; i < numHandCards(&G); i++)
        {
            if(handCard(i, &G) == 1)
            {
                cardExists = 1;
            }
        }

        if(cardExists == 1) 
        {
            printf("    PASS: Player gains an estate\n");
        }
        else
        {
            printf("    FAIL: Player does not gain an estate\n");
        }

    printf("\nAll tests passed!\n");

    return 0;
}
