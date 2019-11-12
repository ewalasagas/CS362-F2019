/* -----------------------------------------------------------------------
 * alasagae - Assignment #3
 * UNIT TEST 5:    MINE FUNCTION TEST
 *
 *
 * Include the following lines in your makefile (if not already included):
 *
 * unittest5: unittest5.c dominion.o rngs.o
 *      gcc -o unittest5 -g  unittest5.c interface.o dominion.o rngs.o $(CFLAGS)
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

// RULE: MINE function tests
//  (1) You may trash a Treasure from your hand
//  (2) If you trash a treasure, you can GAIN a treasure card costing up +3 more than it

int main() {
    int i;
    int seed = 1000;
    int p, r, handCount = 0;

    int k[10] = {adventurer, council_room, mine, treasure_map, minion, remodel, smithy, village, baron, great_hall};
    struct gameState G;

    char cardName[MAX_STRING_LENGTH];
    strcpy(cardName,"");
    cardNumToName(11, cardName);    //mine is card #11

  //  printSupply(&G);

//TEST 1: GET COST OF MINE
        printf("---------------------------------------\n");
        printf("TEST 1: Cost of %s is == 5\n", cardName);
        if(getCost(11) == 5)    //MINE is card #11 
        {
            printf("    PASS: Cost of %s is 5\n", cardName);
        }
        else
        {
            printf("    FAIL: Unable to get cost of %s\n", cardName);
        }

//TEST 2: TRY TO BUY MINE - COST = 5
        //initialize game state and clear 
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(3, k, seed, &G);         // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand
        G.coins = 6;    //Assign coins to 6

        printf("---------------------------------------\n");
        printf("TEST 2: TRY TO BUY MINE\n");

        //printf("Number of coins for player: %d\n", G.coins);    //Check number of coins
        if(buyCard(11, &G) == 0)
        {
            printf("    PASS: Able to buy %s card\n", cardName);
        }
        else
        {
            printf("    FAIL: Unable to buy %s card\n", cardName);
        }

//TEST 3: EXECUTE REFACTORED MIN//TEST 3:   EXECUTE REFACTORED MINE FUNCTION
        printf("---------------------------------------\n");
        printf("TEST 3: EXECUTE REFACTORED MINE FUNCTION (DO NOT TRASH TREASURE CARD)\n");

        //initialize game state and clear 
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(4, k, seed, &G);         // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand

        //Add Mine to hand to test choice 1 of refactored function
        addCardToHand(p, 11, &G);

        //Add SILVER treasure card to hand 
        addCardToHand(p, 5, &G);

        //assign name to card
        char card_trashed[MAX_STRING_LENGTH];
        strcpy(card_trashed,"");
        cardNumToName(5, card_trashed);    //SILVER is card #5

        //  int choice1: using card # to trash
        //  int choice2: card that you want that is +3 or more (must come from supply)
        //  int currentPlayer: p
        //  int handPos: hand position of card 
        if(mine_ref(&G, 5, 6, p, 1) == 0)
        {
            printf("    PASS: %s card executed\n", cardName);
        }
        else
        {
            printf("    FAIL: %s card was not called\n", cardName);
        }

//TEST 4: EXECUTE REFACTORED MIN//TEST 3:   EXECUTE REFACTORED MINE FUNCTION
        printf("---------------------------------------\n");
        printf("TEST 3: EXECUTE REFACTORED MINE FUNCTION (TRASH TREASURE CARD)\n");
        //Get hand position of card that you want to discard -- this is choice 2
        int hand_pos_card;      //declaring variable to ensure position
        for(i = 0; i < numHandCards(&G); i++)
        {
            if(G.hand[p][i] == 5)
            {
                 hand_pos_card = i;
            }
        }
        
        //  int choice1: hand position of card 
        //  int choice2: card that you want that is +3 or more (must come from supply)
        //  int currentPlayer: p
        //  int handPos: hand position of card 
                //assign name to card
        char card_gained[MAX_STRING_LENGTH];
        strcpy(card_gained,"");
        cardNumToName(6, card_gained);    //SILVER is card #5

        //Trying to obtain gold #6
        if(mine_ref(&G, hand_pos_card, 6, p,  hand_pos_card) == 0)
        {
            printf("    PASS: %s card executed\n", cardName);
        }
        else
        {
            printf("    FAIL: %s card was not called\n", cardName);
        }

//TEST 4:   CHECK IF TRASHED CARD WAS REMOVED FROM DECK AND HAND
        printf("---------------------------------------\n");
        printf("TEST 4: CHECK IF TRASHED CARD WAS REMOVED FROM DECK AND HAND\n");
        int confirm_trash = 0;
        for(i = 1; i < numHandCards(&G); i++)
        {
            if(G.hand[p][i] == 5)
            {
                printf("    FAIL: Card %d was not trashed in hand\n", G.hand[p][i]);
                confirm_trash = 1;
            }
            else if (G.deck[p][i] == 5)
            {
                printf("    FAIL: Card %d was not trashed in deck\n", G.deck[p][i]);
                confirm_trash = 1;
            }
        }

        if(confirm_trash == 0)
        {
            printf("    PASS: Card %s was trashed in deck\n", card_trashed);
        }


//TEST 5:   CHECK IF TRASHED CARD WAS REMOVED FROM DECK AND HAND
        printf("---------------------------------------\n");
        printf("TEST 5: CHECK IF GAINED CARD IS IN HAND OR DECK\n");

        int confirm_gained = 0;

        //check in hand
        for(i = 1; i < numHandCards(&G); i++)
        {
            if(G.hand[p][i] == 6)
            {
                confirm_gained = 1;
            }
        }

        //Check in deck
        for(i = 1; i < G.deckCount[p]; i++)
        {
            if (G.deck[p][i] == 6)
            {
                confirm_gained = 1;
            }
        }
         //ASSERT whether card is in deck or hand
        if(confirm_gained == 0)
        {
            printf("    FAIL: Card %s was not gained in deck or hand\n", card_gained);
        }
        else
        {
            printf("    PASS: Card %s was gained in deck or hand\n", card_gained);
        }


    printf("\nAll tests passed!\n");

    return 0;
}
