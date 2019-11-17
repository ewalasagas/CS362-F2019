/*
PURPOSE:
It has a main() function and defines how two players play the 
dominion game with the Smithy and Adventure action card. 
*/


#include "dominion.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char** argv) {
    struct gameState G;
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
                 sea_hag, tribute, smithy};

    printf ("Starting game.\n");


/* ------------------------------------------------------------------
(1) GAME START FUNCTION 
------------------------------------------------------------------ */
    initializeGame(2, k, atoi(argv[1]), &G);
/*
LOCATION: dominion.c
int initializeGame(int numPlayers, int kingdomCards[10], int randomSeed,
                   struct gameState *state) {
------------------------------------------------------------------ */
    int money = 0;
    int smithyPos = -1;
    int adventurerPos = -1;
    int i=0;    //used for index counter

    int numSmithies = 0;
    int numAdventurers = 0;

    while (!isGameOver(&G)) {
        money = 0;
        smithyPos = -1;
        adventurerPos = -1;

    //testing supply card count
        printf("Estate Supply count: %d\n", supplyCount(1, &G));
        printf("Duchy Supply count: %d\n", supplyCount(2, &G));
        printf("Province Supply count: %d\n", supplyCount(3, &G));
        printf("Curse card supply count: %d\n", supplyCount(0, &G));

        for (i = 0; i < numHandCards(&G); i++) {
    /*------------------------------------------------------------------
    int numHandCards(struct gameState *state) 
        {
        return state->handCount[ whoseTurn(state) ];
        }
    ------------------------------------------------------------------ */
            if (handCard(i, &G) == copper)
                money++;
    /*------------------------------------------------------------------
    int handCard(int handPos, struct gameState *state) 
        {
        int currentPlayer = whoseTurn(state);
        return state->hand[currentPlayer][handPos];
        }

    ------------------------------------------------------------------ */
            else if (handCard(i, &G) == silver)
                money += 2;
            else if (handCard(i, &G) == gold)
                money += 3;
            else if (handCard(i, &G) == smithy)
                smithyPos = i;
            else if (handCard(i, &G) == adventurer)
                adventurerPos = i;
        }

        if (whoseTurn(&G) == 0) {
    /*------------------------------------------------------------------
    int whoseTurn(struct gameState *state) 
        {
            return state->whoseTurn;
        }
    ------------------------------------------------------------------*/
            if (smithyPos != -1) {
                printf("0: smithy played from position %d\n", smithyPos);
                playCard(smithyPos, -1, -1, -1, &G);

    /* ------------------------------------------------------------------
    int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state)
    {
        int card;
        int coin_bonus = 0;         //tracks coins gain from actions

        //check if it is the right phase
        if (state->phase != 0)
        {
            return -1;
        }

        //check if player has enough actions
        if ( state->numActions < 1 )
        {
            return -1;
        }

        //get card played
        card = handCard(handPos, state);

        //check if selected card is an action
        if ( card < adventurer || card > treasure_map )
        {
            return -1;
        }

        //play card
        if ( cardEffect(card, choice1, choice2, choice3, state, handPos, &coin_bonus) < 0 )
        {
            return -1;
        }

        //reduce number of actions
        state->numActions--;

        //update coins (Treasure cards may be added with card draws)
        updateCoins(state->whoseTurn, state, coin_bonus);

        return 0;
    }
    ------------------------------------------------------------------ */
                printf("smithy played.\n");
                money = 0;
                i=0;
                while(i<numHandCards(&G)) {
                    if (handCard(i, &G) == copper) {
                        playCard(i, -1, -1, -1, &G);
                        money++;
                    }
                    else if (handCard(i, &G) == silver) {
                        playCard(i, -1, -1, -1, &G);
                        money += 2;
                    }
                    else if (handCard(i, &G) == gold) {
                        playCard(i, -1, -1, -1, &G);
                        money += 3;
                    }
                    i++;
                }
            }

            if (money >= 8) {
                printf("0: bought province\n");
                buyCard(province, &G);
    /* ------------------------------------------------------------------
    int buyCard(int supplyPos, struct gameState *state) 
    {
        int who;
        if (DEBUG) {
            printf("Entering buyCard...\n");
        }

        // I don't know what to do about the phase thing.

        who = state->whoseTurn;

        if (state->numBuys < 1) {
            if (DEBUG)
                printf("You do not have any buys left\n");
            return -1;
        } else if (supplyCount(supplyPos, state) <1) {
            if (DEBUG)
                printf("There are not any of that type of card left\n");
            return -1;
        } else if (state->coins < getCost(supplyPos)) {
            if (DEBUG)
                printf("You do not have enough money to buy that. You have %d coins.\n", state->coins);
            return -1;
        } else {
            state->phase=1;
            //state->supplyCount[supplyPos]--;
            gainCard(supplyPos, state, 0, who); //card goes in discard, this might be wrong.. (2 means goes into hand, 0 goes into discard)

            state->coins = (state->coins) - (getCost(supplyPos));
            state->numBuys--;
            if (DEBUG)
                printf("You bought card number %d for %d coins. You now have %d buys and %d coins.\n", supplyPos, getCost(supplyPos), state->numBuys, state->coins);
        }

        //state->discard[who][state->discardCount[who]] = supplyPos;
        //state->discardCount[who]++;

        return 0;
    }
    ------------------------------------------------------------------ */
            }
            else if (money >= 6) {
                printf("0: bought gold\n");
                buyCard(gold, &G);
            }
            else if ((money >= 4) && (numSmithies < 2)) {
                printf("0: bought smithy\n");
                buyCard(smithy, &G);
                numSmithies++;
            }
            else if (money >= 3) {
                printf("0: bought silver\n");
                buyCard(silver, &G);
            }

            printf("0: end turn\n");
            endTurn(&G);

    /* ------------------------------------------------------------------
    int endTurn(struct gameState *state) 
    {
        int k;
        int i;
        int currentPlayer = whoseTurn(state);

        //Discard hand
        for (i = 0; i < state->handCount[currentPlayer]; i++) {
            state->discard[currentPlayer][state->discardCount[currentPlayer]++] = state->hand[currentPlayer][i];//Discard
            state->hand[currentPlayer][i] = -1;//Set card to -1
        }
        state->handCount[currentPlayer] = 0;//Reset hand count

        //Code for determining the player
        if (currentPlayer < (state->numPlayers - 1)) {
            state->whoseTurn = currentPlayer + 1;//Still safe to increment
        }
        else {
            state->whoseTurn = 0;//Max player has been reached, loop back around to player 1
        }

        state->outpostPlayed = 0;
        state->phase = 0;
        state->numActions = 1;
        state->coins = 0;
        state->numBuys = 1;
        state->playedCardCount = 0;
        state->handCount[state->whoseTurn] = 0;

        //int k; move to top
        //Next player draws hand
        for (k = 0; k < 5; k++) {
            drawCard(state->whoseTurn, state);//Draw a card
        }

        //Update money
        updateCoins(state->whoseTurn, state, 0);

        return 0;
    }
    ------------------------------------------------------------------ */
        }
        else {
            if (adventurerPos != -1) {
                printf("1: adventurer played from position %d\n", adventurerPos);
                playCard(adventurerPos, -1, -1, -1, &G);
                money = 0;
                i=0;
                while(i<numHandCards(&G)) {
                    if (handCard(i, &G) == copper) {
                        playCard(i, -1, -1, -1, &G);
                        money++;
                    }
                    else if (handCard(i, &G) == silver) {
                        playCard(i, -1, -1, -1, &G);
                        money += 2;
                    }
                    else if (handCard(i, &G) == gold) {
                        playCard(i, -1, -1, -1, &G);
                        money += 3;
                    }
                    i++;
                }
            }

            if (money >= 8) {
                printf("1: bought province\n");
                buyCard(province, &G);
            }
            else if ((money >= 6) && (numAdventurers < 2)) {
                printf("1: bought adventurer\n");
                buyCard(adventurer, &G);
                numAdventurers++;
            } else if (money >= 6) {
                printf("1: bought gold\n");
                buyCard(gold, &G);
            }
            else if (money >= 3) {
                printf("1: bought silver\n");
                buyCard(silver, &G);
            }
            printf("1: endTurn\n");

            endTurn(&G);
        }
    } // end of While

    printf ("Finished game.\n");
    printf ("Player 0: %d\nPlayer 1: %d\n", scoreFor(0, &G), scoreFor(1, &G));

    /* ------------------------------------------------------------------
    int scoreFor (int player, struct gameState *state) 
    {
        int i;
        int score = 0;
        //score from hand
        for (i = 0; i < state->handCount[player]; i++)
        {
            if (state->hand[player][i] == curse) {
                score = score - 1;
            };
            if (state->hand[player][i] == estate) {
                score = score + 1;
            };
            if (state->hand[player][i] == duchy) {
                score = score + 3;
            };
            if (state->hand[player][i] == province) {
                score = score + 6;
            };
            if (state->hand[player][i] == great_hall) {
                score = score + 1;
            };
            if (state->hand[player][i] == gardens) {
                score = score + ( fullDeckCount(player, 0, state) / 10 );
            };
        }

        //score from discard
        for (i = 0; i < state->discardCount[player]; i++)
        {
            if (state->discard[player][i] == curse) {
                score = score - 1;
            };
            if (state->discard[player][i] == estate) {
                score = score + 1;
            };
            if (state->discard[player][i] == duchy) {
                score = score + 3;
            };
            if (state->discard[player][i] == province) {
                score = score + 6;
            };
            if (state->discard[player][i] == great_hall) {
                score = score + 1;
            };
            if (state->discard[player][i] == gardens) {
                score = score + ( fullDeckCount(player, 0, state) / 10 );
            };
        }

        //score from deck
        for (i = 0; i < state->discardCount[player]; i++)
        {
            if (state->deck[player][i] == curse) {
                score = score - 1;
            };
            if (state->deck[player][i] == estate) {
                score = score + 1;
            };
            if (state->deck[player][i] == duchy) {
                score = score + 3;
            };
            if (state->deck[player][i] == province) {
                score = score + 6;
            };
            if (state->deck[player][i] == great_hall) {
                score = score + 1;
            };
            if (state->deck[player][i] == gardens) {
                score = score + ( fullDeckCount(player, 0, state) / 10 );
            };
        }

        return score;
    }
    ------------------------------------------------------------------ */

    return 0;
}
