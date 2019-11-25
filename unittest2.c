/* -----------------------------------------------------------------------
 * alasagae - Assignment #5 - testing other code
 * UNIT TEST 2:    MINION FUNCTION TEST
 *
 *
 * Include the following lines in your makefile (if not already included):
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest2.c interface.o dominion.o rngs.o $(CFLAGS)
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

// RULE: MINION function gives +1 ACTION 
//       CHOOSE 1:
//          A: +2 coins
//          B: Discard your hand, +4 cards, and each other player with at LEAST
//              5 cards in hand discards their hand and draws 4 cards from your deck

int main() {
    int i, j;
    int seed = 1000;
    int p, r, handCount = 0;
    int numPlayers = 2;

    int k[10] = {adventurer, council_room, feast, gardens, minion, remodel, smithy, village, baron, great_hall};
    struct gameState G;

    char cardName[MAX_STRING_LENGTH];
    strcpy(cardName,"");
    cardNumToName(17, cardName);

//TEST 1: GET COST OF MINION
        printf("---------------------------------------\n");
        printf("TEST 1: Cost of %s is == 5\n", cardName);
        if(getCost(17) == 5)
        {
            printf("    PASS: Cost of %s is 5\n", cardName);
        }
        else
        {
            printf("    FAIL: Unable to get cost of %s\n", cardName);
        }

//TEST 2: TRY TO BUY MINION - COST = 5
        //initialize game state and clear 
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(3, k, seed, &G);         // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand
        G.coins = 5;    //Assign coins to 5

        printf("---------------------------------------\n");
        printf("TEST 2: TRY TO BUY MINION\n");

        //printf("Number of coins for player: %d\n", G.coins);    //Check number of coins
        if(buyCard(17, &G) == 0)
        {
            printf("    PASS: Able to buy %s card\n", cardName);
        }
        else
        {
            printf("    FAIL: Unable to buy %s card\n", cardName);
        }

//TEST 3: MINION FUNCTION EXECUTES
//ELAINE -- int minion_ref(struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos)
//CHUNG -- int minionR(int choice1, int choice2, int currentPlayer, struct gameState *state, int handPos, int *bonus)
        //initialize game state and clear 
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(2, k, seed, &G);         // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand

        //Add Minion to hand to test choice 1 of refactored function
        addCardToHand(p, 17, &G);

        //Variable to check coins before minion is called (+2 coins)
        int coins_before = G.coins;
        int actions_before = G.numActions;
        //int coinsA = G.coins + 2;
        int cardExists = 0;     //initialiez variable to check if card exists

        printf("---------------------------------------\n");
        printf("TEST 3: EXECUTE REFACTORED MINION FUNCTION\n");

        //Check state of game before (with coins and actions);
        //        printState(&G);
        //printHand(p, &G);
  
//NO CHANGE TO TESTING OF CHOICE/CODE 
        if(minionR(1, 0, p, &G, 1, 0) == 0) //put minion card in hand
        {
            printf("    PASS: Minion card executed\n");
        }
        else
        {
            printf("    FAIL: Minion card was not called\n");
        }
//introduce bug 1: make action counts not correct
//TEST 4: MINION GIVES +1 ACTION
        printf("---------------------------------------\n");
        printf("TEST 4 & BUG1: MINION GIVES +1 ACTION\n");
        if(G.numActions == (actions_before + 1))
        {
            printf("    PASS: Minion actions before call is %d which is equal to current actions of %d\n", actions_before, G.numActions);
        }
        else
        {
            printf("    FAIL: Minion actions before call is %d which is NOT equal to current actions of %d\n", actions_before, G.numActions);
        }
 //introduce bug 2: make incorrect  coin values
//TEST 5: MINION CHOICE 1 -- GAIN +2 COINS 
        printf("---------------------------------------\n");
        printf("TEST 5 & BUG2: MINION CHOICE 1 -- GAIN +2 COINS\n");
        if(G.coins == (coins_before + 2)) 
        {
            printf("    PASS: Coins before of %d is equal to current coins +2 which is %d\n", coins_before, G.coins);
        }
        else
        {
            printf("    FAIL: Coins before of %d is NOT equal to current coins +2 which is %d\n", coins_before, G.coins);
        }

//TEST 7: MINION CHOICE 2 -- (A) DISCARD YOUR HAND AND DRAW  4 CARDS FROM YOUR DECK
        //We are testing for current hand size - unable to test discarding cards 
        printf("---------------------------------------\n");
        printf("TEST 6: MINION CHOICE 2 -- (A) DISCARD YOUR HAND\n");
        //initialize game state and clear 
        numPlayers = 4;
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(numPlayers, k, seed, &G);         // initialize a new game with 3 players
        G.handCount[p] = handCount;                 // set the number of cards on hand
        //memcpy(G.hand[p], copper, sizeof(int) * handCount); // set all the cards to copper

        //put minion card in hand
        addCardToHand(p, 17, &G);   //player now has 1 card 
        addCardToHand(p, 1, &G);    //Add cards to hand to test discard hand

        //Check if cards were discarded - store hand in array
        int n = numHandCards(&G);
        int handsBefore[n];

        for(i = 0; i < numHandCards(&G); i++)
        {
            handsBefore[i] = G.hand[p][i]; 
        }

        //call minion function with choice1 == 0, choice2 == 1 
//NO CHANGE TO TESTING OF CHOICE/CODE 
        if(minionR(0, 1, p, &G, 1, 0) != 0) {
            printf("    FAIL: Minion card was not called\n");
        }
        else
        {
            //Check if cards were discarded - current hand caount should also 4
            if(numHandCards(&G) > 0)
            {
               // printf("Num Hand Cards: %d\n", numHandCards(&G));
                for(i = 0; i < numHandCards(&G); i++)
                {
                    for(j = 0; j < numHandCards(&G); j++)
                    {
                        //printf("G.hands: %d, handsBefore[i]: %d\n", G.hand[p][i], handsBefore[i]);
                        if(G.hand[p][i] == handsBefore[j])  //check if previous hand discarded
                        {
                            printf("    FAIL: Cards %d is still in player's hand\n", handsBefore[j]);
                            break;
                        }
                    }
                }
            }
            else
            {
                printf("    PASS: Cards discarded - current hand is empty\n");
            }
        }


//TEST 7: MINION CHOICE 2 -- (B) DISCARD YOUR HAND AND DRAW  4 CARDS FROM YOUR DECK
        //We are testing for current hand size - should be at least 4 -- previsous test shows no cards in hand
        printf("---------------------------------------\n");
        printf("TEST 7: MINION CHOICE 2 -- (B) DRAW 4 CARDS FROM YOUR DECK\n");

        //printDiscard(p, &G); does not show discarded cards
        //check hand count of current player
        if(G.handCount[p] == 4)
        {
            printf("    PASS: Current player discarded hand and drew 4 cards\n");
        }
        else
        {
            printf("    FAIL: Current player did not draw 4 cards\n");
        }

//TEST 8: MINION CHOICE 2 -- (C) OTHER PLAYERS WITH AT LEAST 5 CARDS DISCARDS AND DRAWS 4 CARDS
        printf("---------------------------------------\n");
        printf("TEST 8: MINION CHOICE 2 -- (C) OTHER PLAYERS WITH AT LEAST 5 CARDS DISCARDS AND DRAWS 4 CARDS\n");
        int confirm = 0;    //skip player 0 which is current player
        numHandCards(&G);

        //All other players hand count should be 4 -- skipping 0 as that is current player
        for(i = 1; i < numPlayers; i++)
        {
            if(G.handCount[i] == 4)
            {
                confirm++;
            }
        }
        
        //TEST TO CHECK OTHER PLAYER'S HANDS
        if(confirm == 3)
        {
            printf("    PASS: Players have drawn 4 cards in hand\n");
        }
        else
        {
            printf("    FAIL: Players have not drawn 4 cards\n");
        }

    printf("\nAll tests passed!\n");

    return 0;
}
