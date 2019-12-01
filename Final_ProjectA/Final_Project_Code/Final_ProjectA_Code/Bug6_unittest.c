/* -----------------------------------------------------------------------
 * alasagae - Final Project
 * BUG6 unittest: The logic in the case statement for feast seems to be incorrect.  
 *      On line 762 Instead of updating the coins tally, it should enter the while loop
 *      and allowing players to gain a card up to 5 coins without taking into account 
 *      how much coins the player has.
 *
 *  Bug6_unittest: Bug6_unittest.c dominion.o rngs.o interface.o
 *      gcc -o Bug6_unittest -g  Bug6_unittest.c interface.o dominion.o rngs.o $(CFLAGS)
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

//FEAST FUNCTION
    //Trash this card
    //Gain a card costing up to 5 cois

int main() {
    int i;
    int seed = 1000;
    int p, r, handCount = 0;

    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int card_in_deck = 0;

    //Initialize to 2 player game and clear game state
    memset(&G, 23, sizeof(struct gameState));   // clear the game state 
    r = initializeGame(2, k, seed, &G);         // initialize a new game 2 player
    G.handCount[p] = handCount;                 // set the number of cards on hand
    //memcpy(G.hand[p], copper, sizeof(int) * handCount); // set all the cards to copper

    //printSupply(&G);

//Call FEAST card: feast is #9
//int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)   //bonus is a pointer to an int    
//Choice1 is the card you want
//NOt USED: Choice2, Choice3, handPos, Bonus


//cardEffect(9, 2, 0, 0, &G, 0, 0);     //gain Duchy in discard check in discard


    char cardName_Duchy[MAX_STRING_LENGTH];
    char cardName_Gold[MAX_STRING_LENGTH];
    char cardName_Gardens[MAX_STRING_LENGTH];
    strcpy(cardName_Duchy,"");
    strcpy(cardName_Gold,"");   
    strcpy(cardName_Gardens,""); 
    cardNumToName(2, cardName_Duchy);
    cardNumToName(6, cardName_Gold);
    cardNumToName(10, cardName_Gardens);   

//TEST1: CHECK IF CAN BUY CARD THAT COST 5 - SHOULD PASS
 printf("---------------------------------------\n");
        printf("TEST 1: CHECK IF CAN BUY CARD THAT COST 5 - BUY DUCHY\n");
        cardEffect(9, 2, 0, 0, &G, 0, 0);     //gain Duchy = cost is 5

        printf("Check discardCount %d\n", G.discardCount[p]);

        for(i = 0; i < G.discardCount[p]; i++)
        {
            if(G.discard[p][i] == 2)
            {
                card_in_deck = 1;
            }
        }

        if(card_in_deck == 1)
        {
            printf("    PASS: Card %s in discard deck of player\n", cardName_Duchy);
        }
        else
        {
            printf("    FAIL: Did not acquire %s\n", cardName_Duchy);
        }
       // printDiscard(p, &G);

        //RESET card_in_deck
        card_in_deck = 0;
//TEST 2: CHECK IF CAN BUY CARD 5+ - SHOULD PASS CONDITION
 printf("---------------------------------------\n");
        printf("TEST 2: CHECK IF CAN BUY CARD THAT COST MORE THAN 5 - BUY GOLD\n");
        cardEffect(9, 6, 0, 0, &G, 0, 0);     //gain Gold = cost is 6

        printf("Check discardCount %d\n", G.discardCount[p]);

        for(i = 0; i < G.discardCount[p]; i++)
        {
            if(G.discard[p][i] == 6)
            {
                card_in_deck = 1;
            }
        }

        if(card_in_deck == 1)
        {
            printf("    FAIL: Card %s in discard deck of player\n", cardName_Gold);
        }
        else
        {
            printf("    PASS: Did not acquire %s\n", cardName_Gold);
        }
       // printDiscard(p, &G);

        //RESET card_in_deck
        card_in_deck = 0;
//TEST 3: CHECK IF CAN BUY CARD THAT COST LESS THAN 5 - SHOULD PASS
 printf("---------------------------------------\n");
        printf("TEST 3: CHECK IF CAN BUY CARD THAT COST LESS THAN 5 - SHOULD PASS\n");
        cardEffect(9, 4, 0, 0, &G, 0, 0);     //gain Gardens = cost is 4

        printf("Check discardCount %d\n", G.discardCount[p]);

        for(i = 0; i < G.discardCount[p]; i++)
        {
            if(G.discard[p][i] == 4)
            {
                card_in_deck = 1;
            }
        }

        if(card_in_deck == 1)
        {
            printf("    PASS: Card %s in discard deck of player\n", cardName_Gardens);
        }
        else
        {
            printf("    FAIL: Did not acquire %s\n", cardName_Gardens);
        }

    printf("\nAll tests passed!\n");

    return 0;
}
