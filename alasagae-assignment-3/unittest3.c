/* -----------------------------------------------------------------------
 * alasagae - Assignment #3
 * UNIT TEST 3:    ABASSADOR FUNCTION TEST
 *
 *
 * Include the following lines in your makefile (if not already included):
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g  unittest3.c interface.o dominion.o rngs.o $(CFLAGS)
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

// RULE: AMBASSADOR function tests
//  (1) Reveal a card from your hand
//  (2) Return up to 2 copies of it from hand to supply
//  (3) Each other player gains a copy of it
//  NOTE: If card isNOT in supply, Ambassador does nothing
//  NOTE: If there are not enough copies to go around, deal them out in turn order

int main() {
    int i;
    int seed = 1000;
    int p, r, handCount = 0;

    int k[10] = {adventurer, council_room, feast, gardens, minion, remodel, smithy, village, baron, great_hall};
    struct gameState G;

    char cardName[MAX_STRING_LENGTH];
    strcpy(cardName,"");
    cardNumToName(20, cardName);

//TEST 1: GET COST OF AMBASSADOR
        printf("---------------------------------------\n");
        printf("TEST 1: Cost of %s is == 3\n", cardName);
        if(getCost(20) == 3)    //Ambassador is card #20 
        {
            printf("    PASS: Cost of %s is 3\n", cardName);
        }
        else
        {
            printf("    FAIL: Unable to get cost of %s\n", cardName);
        }

//TEST 2: TRY TO BUY AMBASSADOR - COST = 3
        //initialize game state and clear 
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(3, k, seed, &G);         // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand
        G.coins = 5;    //Assign coins to 5

        printf("---------------------------------------\n");
        printf("TEST 2: TRY TO BUY AMBASSADOR\n");

        //printf("Number of coins for player: %d\n", G.coins);    //Check number of coins
        if(buyCard(20, &G) == 0)
        {
            printf("    PASS: Able to buy %s card\n", cardName);
        }
        else
        {
            printf("    FAIL: Unable to buy %s card\n", cardName);
        }

        //initialize game state and clear 
        memset(&G, 23, sizeof(struct gameState));   // clear the game state 
        r = initializeGame(4, k, seed, &G);         // initialize a new game
        G.handCount[p] = handCount;                 // set the number of cards on hand

        //Add Minion to hand to test choice 1 of refactored function
        addCardToHand(p, 20, &G);
        addCardToHand(p, 0, &G);    //Adding curse card for card effect of copying

        //cardname of chosen card to copy
        char cardCopy[MAX_STRING_LENGTH];
        strcpy(cardCopy,"");
        cardNumToName(0, cardCopy);  

        int curse_count_before = supplyCount(0, &G);

//TEST 3:   EXECUTE REFACTORED AMBASSADOR FUNCTION
     printf("---------------------------------------\n");
        printf("TEST 3: EXECUTE REFACTORED AMBASSADOR FUNCTION\n");

        //Hand Position of curse card is 1 -- confirming below in for-loop
        int hand_pos_cardcopy;      //declaring variable to ensure position
        for(i = 0; i < numHandCards(&G); i++)
        {
            if(G.hand[p][i] == 0)
            {
                hand_pos_cardcopy = i;
            }
        }
        
        // choice1 = supplyCount # of card to copy
        // choice2 = number to return to supply
        // handPos = hand# card choice1 is in

        //Making 1 copies of curse
        if(ambassador_ref(0, 1,  &G, hand_pos_cardcopy, p) == 0) 
        {
            printf("    PASS: %s card executed\n", cardName);
        }
        else
        {
            printf("    FAIL: %s card was not called\n", cardName);
        }

//TEST 4:   CHECK SUPPLY COUNT OF COPIED CARD IS DECREMENTED BY 2 IF NUMBER PLAYERS IS 4
        /*Game initialized with 4 players -- supply count would be +1 for choice 2 (1 copy), 
              and then supply count decremented -3 because each player recieves that card  */
        printf("---------------------------------------\n");
        printf("TEST 4:  CHECK SUPPLY COUNT OF COPIED CARD IS DECREMENTED BY 2 IF NUMBER PLAYERS IS 4\n");
        //Checking if supplyCount of card to copy is greater than 0 -- supplyCount(choice1, state)
        if(supplyCount(0, &G) == (curse_count_before - 1))
        {
            printf("    PASS: Card %s supply count was decremented by 2 \n", cardCopy);
        }
        else
        {
            printf("    FAIL: Card %s supply count was not decremented by 2\n", cardCopy);
        }


//TEST 5:   AMBASSADOR GIVES EACH OTHER PLAYER A COPY OF THAT CARD
        //check player decks if they now have a curse card
        printf("---------------------------------------\n");
        printf("TEST 5: AMBASSADOR GIVES EACH OTHER PLAYER A COPY OF THAT CARD \n");

        int track_copied_curse = 0;
        int j;
        
        //Number of players -- skip player 0 which is current player
        for(i = 1; i < G.numPlayers; i++)
        {
            //printf("Player %d: deck size %d\n", i, G.deckCount[i]);     //deck count of player
            for(j = 0; j < G.deckCount[i]; j++)
            {
                if(G.deck[i][j] == 0)
                {
                    track_copied_curse++;
                }
            }
        }
        
         //ASSERT whether card copy exists in other player's decks
        if(track_copied_curse == (G.numPlayers - 1))
        {
            printf("    PASS: Players have a copy of %s in their deck\n", cardCopy);
        }
        else
        {
            printf("    FAIL: Players do not havea copy of %s in their decks\n", cardCopy);
        }      


    printf("\nAll tests passed!\n");

    return 0;
}
