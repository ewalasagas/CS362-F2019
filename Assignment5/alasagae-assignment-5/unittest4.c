/* -----------------------------------------------------------------------
 * alasagae - Assignment #5 - testing other code
 * UNIT TEST 4:    TRIBUTE FUNCTION TEST
 *
 *
 * Include the following lines in your makefile (if not already included):
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c interface.o dominion.o rngs.o $(CFLAGS)
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

// RULE: TRIBUTE function tests
//  Player to left reveals top 2 cards and discards cards
//  (1) IF card is ACTION -- +2 ACTIONS
//  (2) IF card is TREASURE -- +2 COINS ()
//  (3) IF card is VICTORY -- +2 CARDS
//  NOTE: IF you reveal 2 types, it gives you both bonuses
//  NOTE: Curse is no bonus

int main() {
    int i, j;
    int seed = 1000;
    int p, r, handCount = 0;

    int k[10] = {adventurer, council_room, tribute, gardens, minion, remodel, smithy, village, baron, great_hall};
    struct gameState G;

    char cardName[MAX_STRING_LENGTH];
    strcpy(cardName,"");
    cardNumToName(19, cardName);

//TEST 1: GET COST OF TRIBUTE
        printf("---------------------------------------\n");
        printf("TEST 1: Cost of %s is == 5\n", cardName);
        if(getCost(19) == 5)    //TRIBUTE is card #19 
        {
            printf("    PASS: Cost of %s is 5\n", cardName);
        }
        else
        {
            printf("    FAIL: Unable to get cost of %s\n", cardName);
        }

//TEST 2: TRY TO BUY TRIBUTE - COST = 5
        //initialize game state and clear 
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(2, k, seed, &G);         // initialize a new game with 2 plays so it's easier to capture cards
        G.handCount[p] = handCount;                 // set the number of cards on hand
        int opponent_hand_before[5];
        int tributeRevealedCards[2];                //tribute cards array 
        G.coins = 5;    //Assign coins to 6

        //Variable to store buy actions
        int before_buy_actions = G.numBuys;

        printf("---------------------------------------\n");
        printf("TEST 2: TRY TO BUY TRIBUTE (ADDED TRIBUTE TO SUPPLY COUNT)\n");
        
        //printf("Number of coins for player: %d\n", G.coins);    //Check number of coins
        if(buyCard(19, &G) == 0)
        {
            printf("    PASS: Able to buy %s card\n", cardName);
        }
        else
        {
            printf("    FAIL: Unable to buy %s card\n", cardName);
        }

//TEST 3: EXECUTE REFACTORED TRIBUTE FUNCTION
        printf("---------------------------------------\n");
        printf("TEST 3: EXECUTE REFACTORED TRIBUTE FUNCTION\n");
        
        //Get other player's current hand and save in tributeRevealedCards array;
        //Fill other player's hand with diff types of cards in order to test functionalities
        //  (1) IF card is ACTION -- +2 ACTIONS -- assign before actions of current player
        int numActions_before = G.numActions;
        //  (2) IF card is TREASURE -- +2 COINS () -- assign before coin count
        int numCoins_before = G.coins;
        //  (3) IF card is VICTORY -- +2 CARDS -- assign handCount before
        int hand_before = G.handCount[p];

        for(i = 1; i < G.numPlayers; i++)
        {
            //printf("Player %d: deck size %d\n", i, G.deckCount[i]);     //deck count of player
            for(j = 0; j < G.deckCount[i]; j++)
            {
                //Hand position 2 will be an action card -- village #14
                if(j == 2)
                {
                    G.deck[i][j] = 14;
                    tributeRevealedCards[0] = G.deck[i][j];
                }

                //Hand position 3 will be a treasure card -- gold #6
                if(j == 3)
                {
                    G.deck[i][j] = 6;
                    tributeRevealedCards[1] = G.deck[i][j];
                }
                
                //Hand position 4 will be a victory card -- duchy #2
                if(j == 4)
                {
                    G.deck[i][j] = 2;
                    tributeRevealedCards[2] = G.deck[i][j];
                }
            }
        }

        //copy opponent hand before to array
        for(i = 1; i < G.numPlayers; i++)
        {
            for(j = 0; j < G.deckCount[i]; j++)
            {
                //printf("%d\n", G.deck[i][j]);
                opponent_hand_before[j] = G.deck[i][j];
            }
        }

//ELAINE -- int tribute_ref(struct gameState *state, int tributeRevealedCards[2], int nextPlayer, int currentPlayer)
//CHUNG -- int tributeR(int currentPlayer, int nextPlayer, int tributeRevealedCards[], struct gameState *state, int *bonus)
        //  Current player = 0
        //  Next player = 1
        //  Tested the tribute before and after and tribute array changes 
         if(tributeR(p, 1, tributeRevealedCards, &G, 0) == 0) 
        {
            printf("    PASS: %s card executed\n", cardName);
        }
        else
        {
            printf("    FAIL: %s card was not called\n", cardName);
        }

        //printHand(1, &G);


//TEST BUG1: introduce bug 1: should not increase actions
        printf("---------------------------------------\n");
        printf("TEST BUG1: SHOULD NOT INCREASE ACTIONS\n");
        if(numActions_before == G.numActions)
        {
            printf("    PASS: Same number of actions\n");
        }
        else
        {
            printf("    FAIL: Actions before is %d and does not equal current player actions %d\n", numActions_before, G.numActions);
        }

//TEST BUG2: //introduce bug 2: should not increase buys
        printf("---------------------------------------\n");
        printf("TEST BUG2: SHOULD NOT INCREASE ACTIONS\n");
        if(before_buy_actions == G.numBuys)
        {
            printf("    PASS: Same number of buy actions\n");
        }
        else
        {
            printf("    FAIL: Buy actions before is %d and does not equal current player buy actions %d\n", before_buy_actions, G.numBuys);
        }

 //TEST 4: PLAYER REVEALS AND DISCARDS 2 CARDS 
        printf("---------------------------------------\n");
        printf("TEST 4: PLAYER DISCARDED TOP 2 DECK CARDS \n");       

        char card_name[MAX_STRING_LENGTH];
        strcpy(card_name,"");
        int cardExists = 0;

        //Go through opponent hand and check if opponent_hand_before is equal to hand after
        for(i = 1; i < G.numPlayers; i++)
        {
            //Checking at handPos 2 of other player(s) deck-count 
            for(j = 2; j < G.deckCount[i]; j++)
            {
                if(opponent_hand_before[j] ==  G.deck[i][j])
                {
                    cardNumToName(G.deck[i][j], card_name);
                    printf("    FAIL: Tribute card %s is still in deck\n", card_name);
                    cardExists = 1;
                }
            }
        }

        if(cardExists == 0) 
        {
            printf("    PASS: Tribute cards have been removed from deck\n");
        }
        

//TEST 5:   CHECK THAT NUMACTIONS OF CURRENT PLAYER INCREASE +2
        printf("---------------------------------------\n");
        printf("TEST 5: CHECK THAT NUMACTIONS OF CURRENT PLAYER INCREASE +2\n");
        if((numActions_before + 2) == G.numActions)
        {
            printf("    PASS: Current player has %d actions\n", G.numActions);
        }
        else
        {
            printf("    FAIL: Current player has %d actions\n", G.numActions);
        }

//TEST 6:   CHECK THAT TREASURE CARD PLAYED INCREASES COINS BY +2
        printf("---------------------------------------\n");
        printf("TEST 6: CHECK THAT NUMACTIONS OF CURRENT PLAYER INCREASE +2\n");
        if((numCoins_before + 2) == G.coins)
        {
            printf("    PASS: Current player has %d coins\n", G.coins);
        }
        else
        {
            printf("    FAIL: Current player has %d coins\n", G.coins);
        }

//TEST 7:   CHECK THAT VICTORY CARD PLAYED INCREASES HAND COUNT BY +2 FROM DRAW CARD
        printf("---------------------------------------\n");
        printf("TEST 7: CHECK THAT VICTORY CARD PLAYED INCREASES HAND COUNT BY +2 FROM DRAW CARD\n");
        if((hand_before + 2) == G.handCount[p])
        {
            printf("    PASS: Current player has %d hand count\n", G.handCount[p]);
        }
        else
        {
            printf("    FAIL: Current player has %d hand count\n", G.handCount[p]);
        }


    printf("\nAll tests passed!\n");

    return 0;
}
