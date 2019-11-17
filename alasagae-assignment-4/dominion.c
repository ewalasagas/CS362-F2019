/*
PURPOSE: 
It defines all the functions to build a dominion game object 
and its contents (e.g., deck of cards, card effect, score, action, etc.).
*/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int compare(const void* a, const void* b) {
    if (*(int*)a > *(int*)b)
        return 1;
    if (*(int*)a < *(int*)b)
        return -1;
    return 0;
}

struct gameState* newGame() {
    struct gameState* g = malloc(sizeof(struct gameState));
    return g;
}

int* kingdomCards(int k1, int k2, int k3, int k4, int k5, int k6, int k7,
                  int k8, int k9, int k10) {
    int* k = malloc(10 * sizeof(int));
    k[0] = k1;
    k[1] = k2;
    k[2] = k3;
    k[3] = k4;
    k[4] = k5;
    k[5] = k6;
    k[6] = k7;
    k[7] = k8;
    k[8] = k9;
    k[9] = k10;
    return k;
}

int initializeGame(int numPlayers, int kingdomCards[10], int randomSeed,
                   struct gameState *state) {
    int i;
    int j;
    int it;

    //set up random number generator
/* ------------------------------------------------------------------
(2 & 3) GAME START FUNCTION 

LOCATION OF FUNCTION: LINE 146 & rngs.c

FUNCTION BELOW:
void SelectStream(int index)
/* ------------------------------------------------------------------
 * Use this function to set the current random number generator
 * stream -- that stream from which the next random number will come.
 * ------------------------------------------------------------------
 */
/*
{
    stream = ((unsigned int) index) % STREAMS;
    if ((initialized == 0) && (stream != 0))   // protect against        
        PlantSeeds(DEFAULT);                     // un-initialized streams 
}
------------------------------------------------------------------ */
    SelectStream(1);                //found in rngs.c
    PutSeed((long)randomSeed);      //found in rngs.c

    //check number of players
    //MAX_PLAYERS found in dominion.h -- #define MAX_PLAYERS 4
    if (numPlayers > MAX_PLAYERS || numPlayers < 2)
    {
        return -1;
    }

    //set number of players
    state->numPlayers = numPlayers;

    //check selected kingdom cards are different
    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            if (j != i && kingdomCards[j] == kingdomCards[i])
            {
                return -1;
            }
        }
    }


    //initialize supply
    ///////////////////////////////

    //set number of Curse cards
    if (numPlayers == 2)
    {
        state->supplyCount[curse] = 10;
    }
    else if (numPlayers == 3)
    {
        state->supplyCount[curse] = 20;
    }
    else
    {
        state->supplyCount[curse] = 30;
    }

    //set number of Victory cards
    if (numPlayers == 2)
    {
        state->supplyCount[estate] = 8;
        state->supplyCount[duchy] = 8;
        state->supplyCount[province] = 8;
    }
    else
    {
        state->supplyCount[estate] = 12;
        state->supplyCount[duchy] = 12;
        state->supplyCount[province] = 12;
    }

    //set number of Treasure cards
    state->supplyCount[copper] = 60 - (7 * numPlayers);
    state->supplyCount[silver] = 40;
    state->supplyCount[gold] = 30;

    //set number of Kingdom cards
    for (i = adventurer; i <= treasure_map; i++)       	//loop all cards
    {
        for (j = 0; j < 10; j++)           		//loop chosen cards
        {
            if (kingdomCards[j] == i)
            {
                //check if card is a 'Victory' Kingdom card
                if (kingdomCards[j] == great_hall || kingdomCards[j] == gardens)
                {
                    if (numPlayers == 2) {
                        state->supplyCount[i] = 8;
                    }
                    else {
                        state->supplyCount[i] = 12;
                    }
                }
                else
                {
                    state->supplyCount[i] = 10;
                }
                break;
            }
            else    //card is not in the set choosen for the game
            {
                state->supplyCount[i] = -1;
            }
        }

    }

    ////////////////////////
    //supply intilization complete

    //set player decks
    for (i = 0; i < numPlayers; i++)
    {
        state->deckCount[i] = 0;
        for (j = 0; j < 3; j++)
        {
            state->deck[i][j] = estate;
            state->deckCount[i]++;
        }
        for (j = 3; j < 10; j++)
        {
            state->deck[i][j] = copper;
            state->deckCount[i]++;
        }
    }

    //shuffle player decks
    for (i = 0; i < numPlayers; i++)
    {
        if ( shuffle(i, state) < 0 )
        {
            return -1;
        }
    }

    //draw player hands
    for (i = 0; i < numPlayers; i++)
    {
        //initialize hand size to zero
        state->handCount[i] = 0;
        state->discardCount[i] = 0;
        //draw 5 cards
         for (j = 0; j < 5; j++)
        	{
        	  drawCard(i, state);
        	}
    }

    //set embargo tokens to 0 for all supply piles
    for (i = 0; i <= treasure_map; i++)
    {
        state->embargoTokens[i] = 0;
    }

    //initialize first player's turn
    state->outpostPlayed = 0;
    state->phase = 0;
    state->numActions = 1;
    state->numBuys = 1;
    state->playedCardCount = 0;
    state->whoseTurn = 0;
    state->handCount[state->whoseTurn] = 0; //hand count is supposed to initalized to 0?
    //int it; move to top

    //Moved draw cards to here, only drawing at the start of a turn
    for (it = 0; it < 5; it++) {
        drawCard(state->whoseTurn, state);
    }

    updateCoins(state->whoseTurn, state, 0);

    return 0;

// Returns -1 on error 
// Returns 0 on success 

}

int shuffle(int player, struct gameState *state) {
    // FUNCTION: Getter for gameState returning the card in a specific position of the hand
    // RETURN: Returns the integer of the card in the handPos of the current player. 

    int newDeck[MAX_DECK];  //MAX_DECK in domnion.h -- #define MAX_DECK 500
    int newDeckPos = 0;
    int card;
    int i;

    if (state->deckCount[player] < 1)
        return -1;
    qsort ((void*)(state->deck[player]), state->deckCount[player], sizeof(int), compare);
    /*
     void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void *, const void*)) 
    */
    /* SORT CARDS IN DECK TO ENSURE DETERMINISM! */

    while (state->deckCount[player] > 0) {
    //floor is <math>
        card = floor(Random() * state->deckCount[player]);
        /*
RANDOM() FUNCTION in rngs.c
double Random(void)
/* ----------------------------------------------------------------
 * Random returns a pseudo-random real number uniformly distributed
 * between 0.0 and 1.0.
 * ----------------------------------------------------------------


 FLOOR() FUNCTION in C library
 The C library function double floor(double x) returns the largest integer value less than or equal to x.
 */
        newDeck[newDeckPos] = state->deck[player][card];
        newDeckPos++;
        for (i = card; i < state->deckCount[player]-1; i++) {
            state->deck[player][i] = state->deck[player][i+1];
        }
        state->deckCount[player]--;
    }
    for (i = 0; i < newDeckPos; i++) {
        state->deck[player][i] = newDeck[i];
        state->deckCount[player]++;
    }

    return 0;
}
/* ----------------------------------------------------------------------------------------------------
(1) Play a card functions
------------------------------------------------------------------------------------------------------- */
int playCard(int handPos, int choice1, int choice2, int choice3, struct gameState *state)
{
    /* FUNCTION: Validates the card being played can be played then calls cardEffect to play the 
                card. */

    // RETURNS: -1 on error 
    // RETURNS: 0 on success 
    int card;
    int coin_bonus = 0; 		//tracks coins gain from actions

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

int buyCard(int supplyPos, struct gameState *state) {
    // FUNCTION: Validates the card being played can be played then calls cardEffect to play the card
    // RETURN: Returns -1 on error
    // RETURN: Returns 0 on success

    int who;    //the current player
    if (DEBUG) {
        printf("Entering buyCard...\n");
    }

    // I don't know what to do about the phase thing.

    who = state->whoseTurn;

    if (state->numBuys < 1) {
    //    if (DEBUG)
            printf("You do not have any buys left\n");
        return -1;
    } else if (supplyCount(supplyPos, state) <1) {
    //    if (DEBUG)
            printf("There are not any of that type of card left\n");
        return -1;
    } else if (state->coins < getCost(supplyPos)) {
    //    if (DEBUG)
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

int numHandCards(struct gameState *state) {
    // FUNCTION: Getter for gameState handCount. 
    return state->handCount[ whoseTurn(state) ];
    // RETURN: Returns number of cards in the current player’s hand 
}
/* ----------------------------------------------------------------------------------------------------
(2) Play a card functions
------------------------------------------------------------------------------------------------------- */
int handCard(int handPos, struct gameState *state) {
    //FUNCTION: Getter for gameState returning the card in a specific position of the hand
    int currentPlayer = whoseTurn(state);
    return state->hand[currentPlayer][handPos];
    //RETURN: Returns the integer of the card in the handPos of the current player.
}

int supplyCount(int card, struct gameState *state) {
    // FUNCTION: Getter for gameState returning the supply count of a specific card
    return state->supplyCount[card];
    // RETURN: Returns the count of supply for a card
}

int fullDeckCount(int player, int card, struct gameState *state) {
    int i;
    int count = 0;

    for (i = 0; i < state->deckCount[player]; i++)
    {
        if (state->deck[player][i] == card) count++;
    }

    for (i = 0; i < state->handCount[player]; i++)
    {
        if (state->hand[player][i] == card) count++;
    }

    for (i = 0; i < state->discardCount[player]; i++)
    {
        if (state->discard[player][i] == card) count++;
    }

    return count;
}
/* ----------------------------------------------------------------------------------------------------
(3) Play a card functions
------------------------------------------------------------------------------------------------------- */
int whoseTurn(struct gameState *state) {
    //FUNCTION: Getter for gameState whoseTurn
    //Returns the integer of whose turn it is now
    return state->whoseTurn;
}


/* ----------------------------------------------------------------------------------------------------
(3) End turn functions
------------------------------------------------------------------------------------------------------- */
int endTurn(struct gameState *state) {
    /* FUNCTION: Changes the current player to the next player or the first player 
    if the last player has finished their turn.  Resets the player gameState and draws 
    cards for the next player. 
    */

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
/* ------------------------------------------------------------------------------------------------------
(9) Play a card function
------------------------------------------------------------------------------------------------------ */
int isGameOver(struct gameState *state) {
    //isGameOver == 1 then game is over
    //isGameOver == 0 then game is NOT over (continue)


    /* FUNCTION: Checks the number of provinces or if three supply piles are empty to determine if the game is over. */
    int i;
    int j;

    //if stack of Province cards is empty, the game ends
    if (state->supplyCount[province] == 0)
    {
        return 1;   //Game ends when 
    }

    //if three supply pile are at 0 3 times (the j++), the game ends
    j = 0;
    for (i = 0; i < 25; i++)
    {
        if (state->supplyCount[i] == 0)
        {
            j++;
        }
    }
    if ( j >= 3)
    {
        return 1;
    }

    return 0;
}

int scoreFor (int player, struct gameState *state) {
    // FUNCTION: Calculates a specific player’s current score
    // RETURNS: returns score
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

int getWinners(int players[MAX_PLAYERS], struct gameState *state) {
    int i;
    int j;
    int highScore;
    int currentPlayer;

    //get score for each player
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        //set unused player scores to -9999
        if (i >= state->numPlayers)
        {
            players[i] = -9999;
        }
        else
        {
            players[i] = scoreFor (i, state);
        }
    }

    //find highest score
    j = 0;
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if (players[i] > players[j])
        {
            j = i;
        }
    }
    highScore = players[j];

    //add 1 to players who had less turns
    currentPlayer = whoseTurn(state);
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if ( players[i] == highScore && i > currentPlayer )
        {
            players[i]++;
        }
    }

    //find new highest score
    j = 0;
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if ( players[i] > players[j] )
        {
            j = i;
        }
    }
    highScore = players[j];

    //set winners in array to 1 and rest to 0
    for (i = 0; i < MAX_PLAYERS; i++)
    {
        if ( players[i] == highScore )
        {
            players[i] = 1;
        }
        else
        {
            players[i] = 0;
        }
    }

    return 0;
}
/* ------------------------------------------------------------------------------
(?) Play a card functions
------------------------------------------------------------------------------ */
int drawCard(int player, struct gameState *state)
{   
    // FUNCTION: Adds a card from the deck to the player’s hand.  If the deck is empty 
    // the deck is shuffled, and if still empty returns -1

    // RETURNS: Returns -1 if the deck is empty after shuffling 
    // RETURNS: Returns 0 on success


    // RULES AS PER PLAYDOM: If deck is empty, discard pile becomes the new deck. Shuffle 
    // once discard pile is moved to deck. Draw first card from the deck into player's hand. 
    //If deck is not empty, get the first card from the deck into hand. 
    int count;
    int deckCounter;
    
    if (state->deckCount[player] <= 0) { //Deck is empty
        //printf("If deck is greater than \n");
        //getchar();

        //Step 1 Shuffle the discard pile back into a deck
        int i;
        //Move discard to deck
        for (i = 0; i < state->discardCount[player]; i++) 
        {
            state->deck[player][i] = state->discard[player][i];
            state->discard[player][i] = -1;
        }

        state->deckCount[player] = state->discardCount[player];
        state->discardCount[player] = 0;//Reset discard

        //Shufffle the deck
        shuffle(player, state);//Shuffle the deck up and make it so that we can draw

        if (DEBUG) { //Debug statements
            printf("Deck count now: %d\n", state->deckCount[player]);
        }

        state->discardCount[player] = 0;

        //Step 2 Draw Card
        count = state->handCount[player];//Get current player's hand count

        if (DEBUG) { //Debug statements
            printf("Current hand count: %d\n", count);
        }

        deckCounter = state->deckCount[player];//Create a holder for the deck count

        if (deckCounter == 0)
            return -1;      //deck is empty after shuffling?

        state->hand[player][count] = state->deck[player][deckCounter - 1];//Add card to hand
        state->deckCount[player]--;
        state->handCount[player]++;//Increment hand count
    }

    else 
    {
        //printf("Deck is less than 0\n");
        //getchar();
        //removed re-initialization of count and deckCounter
        count = state->handCount[player];//Get current hand count for player
        //int deckCounter;          
        if (DEBUG) { //Debug statements
            printf("Current hand count: %d\n", count);
        }

        deckCounter = state->deckCount[player];//Create holder for the deck count
        state->hand[player][count] = state->deck[player][deckCounter - 1];//Add card to the hand
        state->deckCount[player]--;
        state->handCount[player]++;//Increment hand count
    }

    return 0;
}
/* -----------------------------------------------------------------------------------------------------------
(7) Play a card function
----------------------------------------------------------------------------------------------------------- */
int getCost(int cardNumber)
{
    // FUNCTION: Getter for the cost of cards (hard coded into this function). 
    // RETURNS: Returns the cost of the card on success 
    // RETURNS: Returns -1 on failure 

    switch( cardNumber )
    {
    case curse:
        return 0;
    case estate:
        return 2;
    case duchy:
        return 5;
    case province:
        return 8;
    case copper:
        return 0;
    case silver:
        return 3;
    case gold:
        return 6;
    case adventurer:
        return 6;
    case council_room:
        return 5;
    case feast:
        return 4;
    case gardens:
        return 4;
    case mine:
        return 5;
    case remodel:
        return 4;
    case smithy:
        return 4;
    case village:
        return 3;
    case baron:
        return 4;
    case great_hall:
        return 3;
    case minion:
        return 5;
    case steward:
        return 3;
    case tribute:
        return 5;
    case ambassador:
        return 3;
    case cutpurse:
        return 4;
    case embargo:
        return 2;
    case outpost:
        return 5;
    case salvager:
        return 4;
    case sea_hag:
        return 4;
    case treasure_map:
        return 4;
    }

    return -1;
}
//Refactor code: Baron, minion, ambassador, tribute, mine
int baron_ref(struct gameState *state, int choice1, int currentPlayer) {

        //Get current player
        currentPlayer = whoseTurn(state);

        state->numBuys++;//Increase buys by 1!
        if (choice1 > 0) { //Boolean true or going to discard an estate
            int p = 0;//Iterator for hand!
            int card_not_discarded = 1;//Flag for discard set!

            while(card_not_discarded) {    
                if (state->hand[currentPlayer][p] == estate) { //Found an estate card!
                    //printf("Found an estate card\n");
                    //getchar();
                    state->coins += 4;//Add 4 coins to the amount of coins
                    state->discard[currentPlayer][state->discardCount[currentPlayer]] = state->hand[currentPlayer][p];
                    state->discardCount[currentPlayer]++;

                    //Fixed p = 0
                    for (p = 0; p < state->handCount[currentPlayer]; p++) {
                        state->hand[currentPlayer][p] = state->hand[currentPlayer][p+1];
                    }
                    state->hand[currentPlayer][state->handCount[currentPlayer]] = -1;
                    state->handCount[currentPlayer]--;

                    card_not_discarded = 0;//Exit the loop
                }
                else if (p > state->handCount[currentPlayer]) {
                    //printf("Estate card not found\n");
                    //getchar();
                    if(DEBUG) {
                        printf("No estate cards in your hand, invalid choice\n");
                        printf("Must gain an estate if there are any\n");
                    }
                    if (supplyCount(estate, state) > 0) {
                        gainCard(estate, state, 0, currentPlayer);

                        state->supplyCount[estate]--;//Decrement estates
                        if (supplyCount(estate, state) == 0) {
                            isGameOver(state);
                        }
                    }
                    card_not_discarded = 0;//Exit the loop
                }

                else {
                    p++;//Next card
                }
            }
        }

        else {
            //printf("Second choice of not discarding - gain an estate if you have more than 0\n");
            //getchar();
            if (supplyCount(estate, state) > 0) {
            //    printf("Gain an estate\n");
            //    getchar();
                gainCard(estate, state, 2, currentPlayer);//Gain an estate - flag should be 2

                state->supplyCount[estate]--;//Decrement Estates 
                if (supplyCount(estate, state) == 0) {
                    isGameOver(state);
                }
            }
        }

        return 0;
}


int minion_ref(struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos) {
        int i;
        int j;
        //+1 action
        state->numActions++;

        //discard card from hand
        discardCard(handPos, currentPlayer, state, 0);

        if (choice1)
        {
        //    printf("Choice 1 activated\n");
        //    getchar();
            state->coins = state->coins + 2;
        }
        else if (choice2)       //discard hand, redraw 4, other players with 5+ cards discard hand and draw 4
        {
        //    printf("Choice 2 activated\n");
        //    getchar();
            //discard hand
        //    printf("Num hand cards: %d\n", numHandCards(state));
            while(numHandCards(state) > 0)      //NEED TO DISCARD HAND
            {
        //        printf("numHandCards are greater than 0 - discarding cards\n");
        //        getchar();
                discardCard(handPos, currentPlayer, state, 0);
            }

//A            //draw 4
            for (i = 0; i < 4; i++)
            {
        //        printf("Player now draws 4 cards\n");
        //        getchar();
                drawCard(currentPlayer, state);
            }

            //other players discard hand and redraw if hand size > 4
            //printf("Number of players: %d\n", state->numPlayers);
            for (i = 0; i < state->numPlayers; i++)
            {
                //printf("Number of players are greater than 0\n");
                //getchar();

                if (i != currentPlayer)
                {
                    //printf("The player is NOT a current player it is player: %d\n", i);
                    //getchar();

                    //printf("Current player hand size: %d\n", state->handCount[i]);
                    if ( state->handCount[i] > 4 )
                    {
                        //printf("If not current player, and have 4 or more cards of hand size, then..\n");
                       // getchar();
                        //discard hand
                        //printf("Handcount: %d\n", state->handCount[i]);
                        while( state->handCount[i] > 0 )
                        {
                            //printf("While the handcount is greater than 0, discard hand\n");
                            //getchar();

                            discardCard(handPos, i, state, 0);
                        }

                        //draw 4
                        for (j = 0; j < 4; j++)
                        {  
                            //printf("Now drawcard of the current player i?\n");
                            //getchar();

                            drawCard(i, state);
                        }
                    }
                }
            }
/*
            for (i = 0; i < state->numPlayers; i++)
            {
                printf("Player %d handcount %d\n", i, state->handCount[i]);
            }
            */
        }
        return 0;   //if success
}

int ambassador_ref(int choice1, int choice2, struct gameState *state, int handPos, int currentPlayer) {
        int i;
        int j = 0;      //used to check if player has enough cards to discard -- we don't need this?

/* choice1 = hand#, choice2 = number to return to supply */

        //choice 1 is what card in your hand do you want to copy?
        //choice 2 is how many copies you want to copy 
        //handPos check cards in hands and make sure current hand position does not equal 

        if (choice2 > 2 || choice2 < 0)
        {
            //printf(" ERROR: Choice 2 is greater than 2 OR is less than 0 - out of scope\n");
            return -1;
        }

        if (choice1 == handPos)     //cannot choose the same card
        {
            //printf("ERROR: Choice 1 is the same as hand position - that means revealing same card\n");
            return -1;
        }

        //printf("Hand count of player: %d\n", state->handCount[currentPlayer]);
        for (i = 0; i < state->handCount[currentPlayer]; i++)
        {
            //printf("In for loop\n check choice1: %d\n check handPos: %d\n current player's hand and choice: %d\n", choice1, handPos, state->hand[currentPlayer][choice1]);
            //printf("i: %d != handPos: %d && state->hand[currentPlayer][i]: %d  == state->hand[currentPlayer][choice1]: %d && i != choice1: %d\n", i, handPos, state->hand[currentPlayer][i], state->hand[currentPlayer][choice1], choice1);
            if (i != handPos && state->hand[currentPlayer][i] == state->hand[currentPlayer][choice1] && i != choice1) //using bug fix from piazza post
            {
                //printf("i: %d != handPos: %d && state->hand[currentPlayer][i]: %d  == state->hand[currentPlayer][choice1]: %d && i != choice1: %d\n", i, handPos, state->hand[currentPlayer][i], state->hand[currentPlayer][choice1], choice1);
                //printf("If statement activated - i is not handPos, currentPlayer's choice, and choice1, increase j by 1\n");
                //getchar();
                j++;
            }
        }

        if (j < choice2)    //J is less than choice2? 
        {
            //printf("ERROR: Choice is greater than player's hand\n");
            return -1;
        }

        if (DEBUG)
            printf("Player %d reveals card number: %d\n", currentPlayer, state->hand[currentPlayer][choice1]);

        //increase supply count for choosen card by amount being discarded
        //printf("Choice2: %d\n", choice2);
        //printf("Supply count: %d\n", state->supplyCount[state->hand[currentPlayer][choice1]]);
        //printf("state hand count of choice1: %d\n", state->hand[currentPlayer][choice1]);
        //printf("Supply count of ambassador: %d\n", state->supplyCount[curse]);
        state->supplyCount[state->hand[currentPlayer][choice1]] += choice2;
        //printf("Increasing supply count: %d\n", state->supplyCount[state->hand[currentPlayer][choice1]]);
        //each other player gains a copy of revealed card
        for (i = 0; i < state->numPlayers; i++)
        {
            if (i != currentPlayer)
            {
                gainCard(state->hand[currentPlayer][choice1], state, 0, i);
            }
        }

        //discard played card from hand
        discardCard(handPos, currentPlayer, state, 0);

        //trash copies of cards returned to supply
        //printf("Need to trash copies of cards returned to supply pile?");
        for (j = 0; j < choice2; j++)
        {
            for (i = 0; i < state->handCount[currentPlayer]; i++)
            {
                if (state->hand[currentPlayer][i] == state->hand[currentPlayer][choice1])
                {
                    discardCard(i, currentPlayer, state, 1);
                    break;
                }
            }
        }

        return 0;
}

int great_hall_ref(struct gameState *state,int currentPlayer, int handPos) {

        //+1 Card
        drawCard(currentPlayer, state);

        //+1 Actions
        state->numActions++;

        //discard card from hand
        discardCard(handPos, currentPlayer, state, 0);
        return 0;
}

/* REFACTORED MINE
Rule: Trash a Treasure card from your hand, gain a treasure card and put it into your hand
*/
int mine_ref(struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos) {
        int i;
        int j;
        
        j = state->hand[currentPlayer][choice1];  //store card we will trash
        //printf("Choice 1: %d\n", state->hand[currentPlayer][choice1]);

        if (state->hand[currentPlayer][choice1] < copper || state->hand[currentPlayer][choice1] > gold)
        {
            return -1;
        }

        //printf("Choice 2: %d\n", choice2);
        if (choice2 > treasure_map || choice2 < curse)  //make sure not out of bounds - see "interface.c" file
        {
            return -1;
        }

        if ( (getCost(state->hand[currentPlayer][choice1]) + 3) > getCost(choice2) )
        {
            return -1;
        }

        gainCard(choice2, state, 2, currentPlayer);

        //discard card from hand
        discardCard(handPos, currentPlayer, state, 0);

        //discard trashed card
        for (i = 0; i < state->handCount[currentPlayer]; i++)
        {
            if (state->hand[currentPlayer][i] == j)
            {
                discardCard(i, currentPlayer, state, 0);
                break;
            }
        }

        return 0;
}

int tribute_ref(struct gameState *state, int tributeRevealedCards[2], int nextPlayer, int currentPlayer) {
    int i;
    //printf("discard count of nextPlayer: %d\n", state->discardCount[nextPlayer]);
   // printf("deck count of nextPlayer: %d\n", state->deckCount[nextPlayer]);
    //getchar();

       if ((state->discardCount[nextPlayer] + state->deckCount[nextPlayer]) <= 1) {
        //printf("BIG IF STATEMENT: discard count + deck count is less than or equal to 1\n");
       // getchar();
            if (state->deckCount[nextPlayer] > 0) {
               // printf("if statement activated: deck count nextPlayer is > 0\n");       //state->deck[player][i] 
                //getchar();
                //printf("Deck count of nextplayer: %d\n", state->deckCount[nextPlayer]);
                //getchar();
                tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer]-1];
                //printf("Tribute card of 0 is being reassigned to: %d\n", state->deck[nextPlayer][state->deckCount[nextPlayer]-1]);
                //getchar();
                state->deckCount[nextPlayer]--;
                //printf("Check deck count of next player is decremented: %d\n", state->deckCount[nextPlayer]);
                //getchar();
            }
            else if (state->discardCount[nextPlayer] > 0) {
                //printf("Else statement activated: discardCount nextplayer is greater than 0\n");
                //getchar();
                //printf("Discard count of nextplayer: %d\n", state->discardCount[nextPlayer]);
                //getchar();
                tributeRevealedCards[0] = state->discard[nextPlayer][state->discardCount[nextPlayer]-1];
                //printf("Tribute card of 0 being reassigned to: %d\n", state->discard[nextPlayer][state->discardCount[nextPlayer]-1]);
                //getchar();
                state->discardCount[nextPlayer]--;
                //printf("Check discard count of next player is decremented: %d\n", state->discardCount[nextPlayer]);
                //getchar();
            }
            else {
                //No Card to Reveal
                //printf("Else - no card was revealed\n");
                //getchar();
                if (DEBUG) {
                    printf("No cards to reveal\n");
                }
            }
        }
        else
        {
        //printf("BIG IF STATEMENT: Else activated\n");
        //getchar();
            if (state->deckCount[nextPlayer] == 0) {
                //printf("If statement activated because deckount of next player == 0\n");
                //getchar();
                    for (i = 0; i < state->discardCount[nextPlayer]; i++) {
                        //printf("Assign deck %d of nextplayer to discard of nextplayer %d\n", i, state->discard[nextPlayer][i]);
                        //getchar();
                        state->deck[nextPlayer][i] = state->discard[nextPlayer][i];//Move to deck
                        state->deckCount[nextPlayer]++;     //THIS IS WRONG - should be of current player
                        //printf("Check deckcount of nextplayer increases?\n");  
                        //getchar();
                        state->discard[nextPlayer][i] = -1;
                        //printf("resenting discard of next player to -1\n");
                        //getchar();
                        state->discardCount[nextPlayer]--;
                        //printf("Decrementing discardCount of player\n");
                        //getchar();
                    }

                    shuffle(nextPlayer,state);//Shuffle the deck
                }

                //printf("IF statement ended or skipped\n");
                //printf("Checking deckCount of next player: %d\n", state->deckCount[nextPlayer]);
                //getchar();
                tributeRevealedCards[0] = state->deck[nextPlayer][state->deckCount[nextPlayer]-1];
                //printf("Tribute card is being reassigned to deck of next player of deck count: %d\n", state->deck[nextPlayer][state->deckCount[nextPlayer]-1]);
                //getchar();
                state->deck[nextPlayer][state->deckCount[nextPlayer]--] = -1;
                //printf("state->deckCount[nextPlayer] was -- %d\n", state->deckCount[nextPlayer]);
               //getchar();
                state->deckCount[nextPlayer]--;
                tributeRevealedCards[1] = state->deck[nextPlayer][state->deckCount[nextPlayer]-1];
                //printf("Tribute card 1 is being reassigned to deck of next player of deck count: %d\n", state->deck[nextPlayer][state->deckCount[nextPlayer]-1]);
                //getchar();
                state->deck[nextPlayer][state->deckCount[nextPlayer]--] = -1;
               // printf(" state->deck[nextPlayer][state->deckCount[nextPlayer] was -- and assigned -1? %d\n",  state->deck[nextPlayer][state->deckCount[nextPlayer]]);
                //getchar();
                state->deckCount[nextPlayer]--;
                //printf(" state->deckCount[nextPlayer] was -- %d\n",  state->deckCount[nextPlayer]);
                //getchar();
            }

            if (tributeRevealedCards[0] == tributeRevealedCards[1]) { //If we have a duplicate card, just drop one
                //printf("If tribute card 0 is == tribute card 1??\n");     //why should we drop one?
                //getchar();
                state->playedCards[state->playedCardCount] = tributeRevealedCards[1];
                //printf("Assigned state->playedCards[state->playedCardCount] to tribute card of 1: %d\n", tributeRevealedCards[1]);
                //getchar();
                state->playedCardCount++;
                //printf("Increasing playCardCount %d\n", state->playedCardCount);
                //getchar();
                tributeRevealedCards[1] = -1;
                //printf("Now reassign tribute cards to -1\n");
                //getchar();
            }


            for (i = 0; i <= 2; i++) {
               // printf("Entering the 4 loop:\n");
                //getchar();
                if (tributeRevealedCards[i] == copper || tributeRevealedCards[i] == silver || tributeRevealedCards[i] == gold) { //Treasure cards
                    //printf("Checking if tribute card i is a treasure card - then add\n");
                    //getchar();
                    state->coins += 2;
                }

                else if (tributeRevealedCards[i] == estate || tributeRevealedCards[i] == duchy || tributeRevealedCards[i] == province || tributeRevealedCards[i] == gardens || tributeRevealedCards[i] == great_hall) { //Victory Card Found
                   //printf("Checking if tribute card is victory - then draw a card\n");
                   //getchar();
                    drawCard(currentPlayer, state);
                    //drawCard(currentPlayer, state);       I don't think we need to draw twice?
                }
                else { //Action Card
                    //printf("Else then it's an aciont card - so add 2 actions\n");
                    //getchar();
                    state->numActions = state->numActions + 2;
                }
            }
        return 0;
}
/* --------------------------------------------------------------------------------------------
(4) Play a card function
-------------------------------------------------------------------------------------------- */
int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)   //bonus is a pointer to an int
{
    // FUNCTION: Executes a card based on the provided card and choices. 
    // RETURNS: Returns -1 if the card failed to play
    // RETURNS: Returns 0 on success

    int i;
    int j;
    int k;
    int x;
    int index;
    int currentPlayer = whoseTurn(state);
    int nextPlayer = currentPlayer + 1;

    int tributeRevealedCards[2] = {-1, -1};
    int temphand[MAX_HAND];// moved above the if statements -- #define MAX_HAND 500
    int drawntreasure=0;
    int cardDrawn;
    int z = 0;// this is the counter for the temp hand
    if (nextPlayer > (state->numPlayers - 1)) {
        nextPlayer = 0;
    }


    //uses switch to select card and perform actions
    switch( card )
    {
    case adventurer:
        while(drawntreasure<2) {
            if (state->deckCount[currentPlayer] <1) { //if the deck is empty we need to shuffle discard and add to deck
                shuffle(currentPlayer, state);
            }
            drawCard(currentPlayer, state);
            cardDrawn = state->hand[currentPlayer][state->handCount[currentPlayer]-1];//top card of hand is most recently drawn card.
            if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
                drawntreasure++;
            else {
                temphand[z]=cardDrawn;
                state->handCount[currentPlayer]--; //this should just remove the top card (the most recently drawn one).
                z++;
            }
        }
        while(z-1>=0) {
            state->discard[currentPlayer][state->discardCount[currentPlayer]++]=temphand[z-1]; // discard all cards in play that have been drawn
            z=z-1;
        }
        return 0;
/*
(1) PLAY COUNCIL ROOM:
Rule: draw 4 cards, increase buys by 1, and each other player draws a card
*/
    case council_room:
        //+4 Cards
        for (i = 0; i < 4; i++)
        {
            drawCard(currentPlayer, state);
        }

        //+1 Buy
        state->numBuys++;

        //Each other player draws a card
        for (i = 0; i < state->numPlayers; i++)
        {
            if ( i != currentPlayer )
            {
                drawCard(i, state);
            }
        }

        //put played card in played card pile
        discardCard(handPos, currentPlayer, state, 0);

        return 0;

    case feast:
        //gain card with cost up to 5
        //Backup hand
        for (i = 0; i <= state->handCount[currentPlayer]; i++) {
            temphand[i] = state->hand[currentPlayer][i];//Backup card
            state->hand[currentPlayer][i] = -1;//Set to nothing
        }
        //Backup hand

        //Update Coins for Buy
        updateCoins(currentPlayer, state, 5);
        x = 1;//Condition to loop on
        while( x == 1) {//Buy one card
            if (supplyCount(choice1, state) <= 0) {
                if (DEBUG)
                    printf("None of that card left, sorry!\n");

                if (DEBUG) {
                    printf("Cards Left: %d\n", supplyCount(choice1, state));
                }
            }
            else if (state->coins < getCost(choice1)) {
                printf("That card is too expensive!\n");

                if (DEBUG) {
                    printf("Coins: %d < %d\n", state->coins, getCost(choice1));
                }
            }
            else {

                if (DEBUG) {
                    printf("Deck Count: %d\n", state->handCount[currentPlayer] + state->deckCount[currentPlayer] + state->discardCount[currentPlayer]);
                }

                gainCard(choice1, state, 0, currentPlayer);//Gain the card
                x = 0;//No more buying cards

                if (DEBUG) {
                    printf("Deck Count: %d\n", state->handCount[currentPlayer] + state->deckCount[currentPlayer] + state->discardCount[currentPlayer]);
                }

            }
        }

        //Reset Hand
        for (i = 0; i <= state->handCount[currentPlayer]; i++) {
            state->hand[currentPlayer][i] = temphand[i];
            temphand[i] = -1;
        }
        //Reset Hand

        return 0;

    case gardens:
    //Called outside function - should be seperate?
    //Action of card -- "Worth 1 VP per 10 cards you have (rounded down)"
        return -1;

/*
(1) PLAY MINE:
Rule: Trash a Treasure card from your hand, gain a treasure card and put it into your hand 
*/
    case mine:
    //REFACTORED
        mine_ref(state, choice1, choice2, currentPlayer, handPos);

    case remodel:
        j = state->hand[currentPlayer][choice1];  //store card we will trash

        if ( (getCost(state->hand[currentPlayer][choice1]) + 2) > getCost(choice2) )
        {
            return -1;
        }

        gainCard(choice2, state, 0, currentPlayer);

        //discard card from hand
        discardCard(handPos, currentPlayer, state, 0);

        //discard trashed card
        for (i = 0; i < state->handCount[currentPlayer]; i++)
        {
            if (state->hand[currentPlayer][i] == j)
            {
                discardCard(i, currentPlayer, state, 0);
                break;
            }
        }


        return 0;
//SMITHY is OK
    case smithy:
        //+3 Cards
        for (i = 0; i < 3; i++)
        {
            drawCard(currentPlayer, state);
        }

        //discard card from hand
        discardCard(handPos, currentPlayer, state, 0);
        return 0;

//VILLAGE is OK
    case village:
        //+1 Card
        drawCard(currentPlayer, state);

        //+2 Actions
        state->numActions = state->numActions + 2;

        //discard played card from hand
        discardCard(handPos, currentPlayer, state, 0);
        return 0;

    case baron:
    //REFACTORED - need parameters from switch statement in order to call them into the function
        baron_ref(state, choice1, currentPlayer);

    case great_hall:
    //REFACTORED - needed to be outside of switch case because it is called outside of cardEFfect function
        great_hall_ref(state, currentPlayer, handPos);

    case minion:
    //REFACTORED - minion(struct gameState *state, int choice1, int choice2, int currentPlayer, int handPos)
        minion_ref(state, choice1, choice2, currentPlayer, handPos);


//STEWARD
//RULE: choose one from the following: draw 2 cards, gain 2 coins, or trash 2 cards from your hand 
    case steward:
        if (choice1 == 1)
        {
            //+2 cards
            drawCard(currentPlayer, state);
            drawCard(currentPlayer, state);
        }
        else if (choice1 == 2)
        {
            //+2 coins
            state->coins = state->coins + 2;
        }
        else
        {
            //trash 2 cards in hand
            discardCard(choice2, currentPlayer, state, 1);
            discardCard(choice3, currentPlayer, state, 1);
        }

        //discard card from hand
        discardCard(handPos, currentPlayer, state, 0);
        return 0;

    case tribute:
    //REFACTORED
        tribute_ref(state, &tributeRevealedCards[2], nextPlayer, currentPlayer);


    case ambassador:
    //REFACTORED
        ambassador_ref(choice1, choice2, state, handPos, currentPlayer);


    case cutpurse:

        updateCoins(currentPlayer, state, 2);
        for (i = 0; i < state->numPlayers; i++)
        {
            if (i != currentPlayer)
            {
                for (j = 0; j < state->handCount[i]; j++)
                {
                    if (state->hand[i][j] == copper)
                    {
                        discardCard(j, i, state, 0);
                        break;
                    }
                    if (j == state->handCount[i])
                    {
                        for (k = 0; k < state->handCount[i]; k++)
                        {
                            if (DEBUG)
                                printf("Player %d reveals card number %d\n", i, state->hand[i][k]);
                        }
                        break;
                    }
                }

            }

        }

        //discard played card from hand
        discardCard(handPos, currentPlayer, state, 0);

        return 0;


    case embargo:
        //+2 Coins
        state->coins = state->coins + 2;

        //see if selected pile is in play
        if ( state->supplyCount[choice1] == -1 )
        {
            return -1;
        }

        //add embargo token to selected supply pile
        state->embargoTokens[choice1]++;

        //trash card
        discardCard(handPos, currentPlayer, state, 1);
        return 0;

    case outpost:
        //set outpost flag
        state->outpostPlayed++;

        //discard card
        discardCard(handPos, currentPlayer, state, 0);
        return 0;

    case salvager:
        //+1 buy
        state->numBuys++;

        if (choice1)
        {
            //gain coins equal to trashed card
            state->coins = state->coins + getCost( handCard(choice1, state) );
            //trash card
            discardCard(choice1, currentPlayer, state, 1);
        }

        //discard card
        discardCard(handPos, currentPlayer, state, 0);
        return 0;

    case sea_hag:
        for (i = 0; i < state->numPlayers; i++) {
            if (i != currentPlayer) {
                state->discard[i][state->discardCount[i]] = state->deck[i][state->deckCount[i]--];
                state->deckCount[i]--;
                state->discardCount[i]++;
                state->deck[i][state->deckCount[i]--] = curse;//Top card now a curse
            }
        }
        return 0;

    case treasure_map:
        //search hand for another treasure_map
        index = -1;
        for (i = 0; i < state->handCount[currentPlayer]; i++)
        {
            if (state->hand[currentPlayer][i] == treasure_map && i != handPos)
            {
                index = i;
                break;
            }
        }
        if (index > -1)
        {
            //trash both treasure cards
            discardCard(handPos, currentPlayer, state, 1);
            discardCard(index, currentPlayer, state, 1);

            //gain 4 Gold cards
            for (i = 0; i < 4; i++)
            {
                gainCard(gold, state, 1, currentPlayer);
            }

            //return success
            return 1;
        }

        //no second treasure_map found in hand
        return -1;
    }

    return -1;
}

/* -----------------------------------------------------------------------------------------------------------
(6) Play a card function
-----------------------------------------------------------------------------------------------------------*/ 
int discardCard(int handPos, int currentPlayer, struct gameState *state,  int trashFlag)
{
    /* FUNCTION: Validates the card being played can be played then calls cardEffect to play the 
                    card */
    // RETURNS: Returns 0 on success 

    //if card is not trashed, added to Played pile
    if (trashFlag < 1)
    {
        //add card to played pile
        state->playedCards[state->playedCardCount] = state->hand[currentPlayer][handPos];
        state->playedCardCount++;
    }

    //set played card to -1
    state->hand[currentPlayer][handPos] = -1;

    //remove card from player's hand
    if ( handPos == (state->handCount[currentPlayer] - 1) )     //last card in hand array is played
    {
        //reduce number of cards in hand
        state->handCount[currentPlayer]--;
    }
    else if ( state->handCount[currentPlayer] == 1 ) //only one card in hand
    {
        //reduce number of cards in hand
        state->handCount[currentPlayer]--;
    }
    else
    {
        //replace discarded card with last card in hand
        state->hand[currentPlayer][handPos] = state->hand[currentPlayer][ (state->handCount[currentPlayer] - 1)];
        //set last card to -1
        state->hand[currentPlayer][state->handCount[currentPlayer] - 1] = -1;
        //reduce number of cards in hand
        state->handCount[currentPlayer]--;
    }

    return 0;
}

/**************** ADDING TRASH CARD FUNCTION ********************************
int trashCard(int handPos, int currentPlayer, struct gameState *state)
{
    //add card to trash pile
    state->trash[state->trashedCardCount] = state->hand[currentPlayer][handPos];
    state->trashedCardCount++;

    //remove played card from player's hand
    //set played card to -1
    state->hand[currentPlayer][handPos] = -1;

    //if trashed card is not the last card in the player's hand or the only card in the player's hand -> move cards up to fill gap
    if ((handPos != (state->handCount[currentPlayer] -1 )) && (state->handCount[currentPlayer] != 1))
    {<!-- -->
    //need to maintain hand order -> set hand[p] = hand[p+1]
    for (int p = handPos; p < state->handCount[currentPlayer]; p++)
    {<!-- -->
        state->hand[currentPlayer][p] = state->hand[currentPlayer][p + 1];
    }
    state->hand[currentPlayer][state->handCount[currentPlayer]] = -1;
    }

    //reduce number of cards in hand
    state->handCount[currentPlayer]--;

    return 0;
}
**/
/* ------------------------------------------------------------------------------------------------------------
(8) Play a card functions
------------------------------------------------------------------------------------------------------------ */
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player)
{
    // FUNCTION: Adds a specific card to a players hand, deck, or trash.  Checks for enough supply of 
    //           the card then puts the card in the directed location

    // RETURN: Returns -1 if not enough supply
    // RETURN: Returns 0 on success


    //Note: supplyPos is enum of choosen card (enumerated card)
    // toFlag - where to put the card
    // player - who to give it to

    //check if supply pile is empty (0) or card is not used in game (-1)
    if ( supplyCount(supplyPos, state) < 1 )
    {
        return -1;
    }

    //added card for [whoseTurn] current player:
    // toFlag = 0 : add to discard
    // toFlag = 1 : add to deck
    // toFlag = 2 : add to hand

    if (toFlag == 1)
    {
        //printf("Flag 1 called\n");
        state->deck[ player ][ state->deckCount[player] ] = supplyPos;
        state->deckCount[player]++;
    }
    else if (toFlag == 2)
    {
        //printf("Flag 2 called\n");        
        state->hand[ player ][ state->handCount[player] ] = supplyPos;
        state->handCount[player]++;
    }
    else
    {
        //printf("Flag 0 called\n");
        state->discard[player][ state->discardCount[player] ] = supplyPos;
        state->discardCount[player]++;
    }

    //decrease number in supply pile
   // state->supplyCount[supplyPos]--;

    return 0;
}
/* -----------------------------------------------------------------------------------------
(5) GAME CARD FUNCTION:
----------------------------------------------------------------------------------------*/

int updateCoins(int player, struct gameState *state, int bonus)
{
    // FUNCTION: Returns the count of a specific card in a specific player’s discard deck and hand. 
    // RETURNS: Returns 0 on success

    int i;

    //reset coin count
    state->coins = 0;

    //add coins for each Treasure card in player's hand
    for (i = 0; i < state->handCount[player]; i++)
    {
        if (state->hand[player][i] == copper)
        {
            state->coins += 1;
        }
        else if (state->hand[player][i] == silver)
        {
            state->coins += 2;
        }
        else if (state->hand[player][i] == gold)
        {
            state->coins += 3;
        }
    }

    //add bonus
    state->coins += bonus;

    return 0;
}


//end of dominion.c
