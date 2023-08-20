


#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// global static constants to hold magic numbers
static const int MAX_CARDS_OF_DECK = 52;
static const int MAX_CARDS_OF_SUIT = 13;
static const int MAX_CARDS_OF_PLAYER_DECK = 12;
static const int MAX_SCORE_DEALER_NEED_STAND = 17;
static const int MAX_SUIT = 4;
static const int BLACKJACK_TARGET_SCORE = 21;

// global static constants to hold advice cutoffs

static const int NORMALCUTOFF = 17;
static const int BIGHANDCUTOFF = 15;
static const int HUGEHANDCUTOFF = 13;
static const int DEFINATECUTOFF = 11;

// global static constants used when returning win/loss
static const int PLAYERWIN = 1;
static const int DEALERWIN = -1;
static const int TIE = 0;

struct Card
{
    string name;        // stores the name of the suit
    string description; // description of the card
    int rank;           // rank of the card (1-13)
    int value;          // value of the card (1-11)

    // constructor, sets initial values to empty/zero
    Card()
    {
        name = "";
        description = "";
        rank = 0;
        value = 0;
    }
};

struct CardArray
{
    Card* pArray; // pointer to an array of cards
    int maxNumber; // max number of cards in a deck
    int currentNumber; // current number of cards in deck

    // constructor, sets initial values to null/zero
    CardArray()
    {
        pArray = nullptr;
        maxNumber = 0;
        currentNumber = 0;
    }
};

// Function to Create a Deck of Cards
void getNewDeck(CardArray& deck)
{
    // set the current and max cards to 52 through passbyreference
    deck.currentNumber = MAX_CARDS_OF_DECK;
    deck.maxNumber = MAX_CARDS_OF_DECK;

    // assign pArray the address of an array of cards
    deck.pArray = new Card[MAX_CARDS_OF_DECK];

    // run a forloop 4 times, once for each suit
    for (int suit = 0; suit < 4; suit++)
    {
        string cardName = "";

        // sets the suits of the cards
        if (suit == 0) { cardName = "S"; } // Spade
        else if (suit == 1) { cardName = "H"; } // Heart
        else if (suit == 2) { cardName = "D"; } // Diamond
        else if (suit == 3) { cardName = "C"; } // Club

        // forloop that creates cards for ranks 1-13
        for (int rank = 0; rank < MAX_CARDS_OF_SUIT; rank++)
        {
            // index to keep track of which card this is in the array
            int index = suit * MAX_CARDS_OF_SUIT + rank;

            // set description to suit + rank
            deck.pArray[index].description = cardName + to_string(rank + 1);

            // set rank and value values
            deck.pArray[index].rank = rank + 1;
            if (rank == 0)
            {
                deck.pArray[index].value = 11;
            }
            else if (rank < 10)
            {
                deck.pArray[index].value = rank + 1;
            }
            else
            {
                deck.pArray[index].value = 10;
            }

            // add the card rank to the card name
            if (rank == 0) { deck.pArray[index].name = "A"; }
            else if (rank < 10) { deck.pArray[index].name = to_string(rank + 1); }
            else if (rank == 10) { deck.pArray[index].name = "J"; }
            else if (rank == 11) { deck.pArray[index].name = "Q"; }
            else if (rank == 12) { deck.pArray[index].name = "K"; }

            // add the suit to the card name
            deck.pArray[index].name += cardName;
        }
    }
}

// function for initializing player's cards
void initPlayerCards(CardArray& cards)
{
    cards.currentNumber = 0;
    cards.maxNumber = MAX_CARDS_OF_PLAYER_DECK;
    cards.pArray = new Card[MAX_CARDS_OF_PLAYER_DECK];
}

// function for initializing dealer's cards
void initDealerCards(CardArray& cards)
{
    initPlayerCards(cards);
}

// function for releasing memory held by dynamic array
void releaseMemory(CardArray& cards)
{
    if (cards.pArray != nullptr)
    {
        delete[] cards.pArray;
        cards.pArray = nullptr;
    }
}


// function to shuffle the cards
void shuffleDeck(CardArray& deck)
{
    // run through the entire array
    for (int index = 0; index < MAX_CARDS_OF_DECK; index++)
    {
        // index of the spot that the card will be shuffled to(randomly generated)
        srand(time(NULL));
        int targetIndex = std::rand() % 51 + 1;

        if (targetIndex != index)
        {
            // swap the cards between deck.pArray[index] and deck.pArray[targetIndex]
            Card temp = deck.pArray[index];
            deck.pArray[index] = deck.pArray[targetIndex];
            deck.pArray[targetIndex] = temp;
        }
    }
}

// Function to print out a deck of cards
void printDeck(const CardArray& deck)
{
    // run the forloop 4 times, 1 for each suit
    for (int suit = 0; suit < 4; suit++)
    {
        // run through all the cards in the suit
        for (int rank = 0; rank < MAX_CARDS_OF_SUIT; rank++)
        {
            // get the index of the card
            int index = suit * MAX_CARDS_OF_SUIT + rank;

            // print out the cards, spaced out 4 apart
            cout << setw(4);
            cout << deck.pArray[index].name;
        }

        cout << endl;
    }

    cout << endl;
}

// Function to print out an array of cards
void printCards(string name, const CardArray& cards)
{
    cout << name;

    for (int i = 0; i < cards.currentNumber; i++)
    {
        cout << setw(4);
        cout << cards.pArray[i].name;
    }

    cout << endl;
}

// Deal a card from the given deck
Card dealCard(CardArray& deck)
{
    Card card = deck.pArray[MAX_CARDS_OF_DECK - deck.currentNumber];

    deck.currentNumber--;

    if (deck.currentNumber == 0)
    {
        // if the deck runs out of cards, replace with a fresh shuffled deck
        getNewDeck(deck);
        shuffleDeck(deck);
    }

    return card;
}

// function for adding a card to a deck
void addCard(CardArray& deck, Card card)
{
    deck.pArray[deck.currentNumber] = card;
    deck.currentNumber++;
}

// function for retrieving the score
int getScore(const CardArray& deck)
{
    int score = 0;
    int numberOfAce = 0;
    //  go thru the deck and add up card values
    for (int i = 0; i < deck.currentNumber; i++)
    {
        // Treat aces as 11s
        if (deck.pArray[i].value == 1)
        {
            score += 11;
            numberOfAce++;
        }
        else
        {
            score += deck.pArray[i].value;
        }
    }

    // treat aces as 1s instead when the score is over 21
    while (score > BLACKJACK_TARGET_SCORE && numberOfAce > 0)
    {
        score -= 10;
        numberOfAce--;
    }

    return score;
}

// function for checking win loss
bool checkWinOrLose(const CardArray& playerCards, const CardArray& dealerCards, bool dealingToPlayer, int& result)
{
    bool gameOver = false;

    int playerScore = getScore(playerCards);
    int dealerScore = getScore(dealerCards);



    // compare player and dealer scores, setting result and output depending on who won
    // adjust the score tracking variables depending on the result
    if (playerScore == dealerScore)
    {
        // if the dealer chooses to stand
        if (playerScore >= MAX_SCORE_DEALER_NEED_STAND)
        {
            // if the player has decided to stand
            if (dealingToPlayer != true)
            {
                printCards("*Dealer*: ", dealerCards);
                cout << endl << "player score = " << playerScore << ", dealer score = " << dealerScore << endl;
                cout << "Game is tied" << endl;
                gameOver = true;
                result = 0;
            }
            else
            {
                // the game continues otherwise, as the player can still add more cards
            }
        }
    }
    else if (playerScore == 21)
    {
        cout << endl << " Blackjack! You win." << endl;
        gameOver = true;
        result = PLAYERWIN;
    }
    else if (dealerScore == 21)
    {
        printCards("*Dealer*: ", dealerCards);
        cout << endl << "Dealer blackjack, You lose." << endl;
        gameOver = true;
        result = DEALERWIN;
    }
    else if (playerScore > 21)
    {
        cout << endl << "Bust! You lose." << endl;
        gameOver = true;
        result = DEALERWIN;
    }
    else if (dealerScore > 21)
    {
        cout << endl << "Dealer is bust, you win." << endl;
        gameOver = true;
        result = PLAYERWIN;
    }

    // returns the state of the game: game keeps going if noone is bust
    // but ends with a w/l if one of the players has gone bust or hit a blackjack
    return gameOver;
}

// function for dealing the first card
void dealFirstCard(CardArray& deck, CardArray& playerCards, CardArray& dealerCards)
{
    cout << endl << "Deal first card" << endl << "---------------" << endl;

    Card playerCard = dealCard(deck);
    Card dealerCard = dealCard(deck);

    // add card to dealer/player's deck
    addCard(dealerCards, dealerCard);
    addCard(playerCards, playerCard);

    // print player/dealer's first card
    printCards("+Player+: ", playerCards);
    printCards("*Dealer*: ", dealerCards);
    cout << endl;
}

// function for dealing the second card
void dealSecondCard(CardArray& deck, CardArray& playerCards, CardArray& dealerCards)
{
    cout << "Deal second card" << endl << "---------------" << endl;

    Card playerCard = dealCard(deck);
    Card dealerCard = dealCard(deck);

    // add card to dealer/player's deck
    addCard(dealerCards, dealerCard);
    addCard(playerCards, playerCard);

    // print out the player's cards
    printCards("+Player+: ", playerCards);

    // print out the dealer's first card and hidden card
    cout << "*Dealer*: ";
    cout << setw(4);
    cout << dealerCards.pArray[0].name;
    cout << "  ??" << endl;

    cout << endl;
}

// function for processing cases where neither dealer or player go bust
int processNoneGoBustCase(const CardArray& playerCards, const CardArray& dealerCards)
{
    int result = 0;
    int playerScore = getScore(playerCards);
    int dealerScore = getScore(dealerCards);

    cout << endl << "player score = " << playerScore << ", dealer score = " << dealerScore << endl;

    // compare totals and award the win to whoever has a higher number
    if (playerScore > dealerScore)
    {
        result = PLAYERWIN;
        cout << "You win!" << endl;
    }
    else if (playerScore == dealerScore)
    {
        result = TIE;
        cout << "Game is tied." << endl;
    }
    else
    {
        result = DEALERWIN;
        cout << "You lose." << endl;
    }

    return result;
}

// function that checks score and gives advice to the player
void provideAdvice(CardArray& cards)
{
    int score = getScore(cards);

    if (score >= NORMALCUTOFF)
    {
        cout << endl <<  "ADVICE: ";
        cout << endl << "you should stand" << endl;
        cout << "your score is " << score << ", if you hit you are likely to go bust" << endl;
    }
    else if (score >= BIGHANDCUTOFF && cards.currentNumber >= 4)
    {
        // score >= 15 and has >= 4 cards
        cout << endl << "ADVICE: ";
        cout << endl << "you should stand" << endl;
        cout << "your score is " << score << " and you only have " << cards.currentNumber << " cards, if you hit you are likely to go bust" << endl;
    }
    else if (score >= HUGEHANDCUTOFF && cards.currentNumber >= 6)
    {
        // score >= 13 and has >= 6 cards, that means most of small rank cards have been dealt, the possibility of getting a higher rank card is higher
        cout << endl << "ADVICE: ";
        cout << endl << "you should stand" << endl;
        cout << "your score is " << score << ", but you have already dealt " << cards.currentNumber << " cards, if you hit you are likely to go bust" << endl;
    }
    else if (score >= DEFINATECUTOFF)
    {
        cout << endl << "ADVICE: ";
        cout << endl << "you should likely hit" << endl;
        cout << "your score is " << score << " and you only have " << cards.currentNumber << " cards" << endl;
    }
    else
    {
        cout << endl << "ADVICE: ";
        cout << endl << "you have no chance of going bust, you should definately hit" << endl;
        cout << "your score is " << score << " and you only have " << cards.currentNumber << " cards" << endl;
    }
}


// function for playing blackjack
int blackjack(CardArray& deck)
{
    // initialize cards for player and dealer
    CardArray dealerCards;
    CardArray playerCards;

    initDealerCards(dealerCards);
    initPlayerCards(playerCards);

    int result = 0;
    bool gameOver = false;
    Card playerCard;
    Card dealerCard;

    do
    {
        // deal initial 2 cards
        dealFirstCard(deck, playerCards, dealerCards);

        dealSecondCard(deck, playerCards, dealerCards);

        // check if any blackjacks or busts have ended the game
        gameOver = checkWinOrLose(playerCards, dealerCards, true, result);

        if (gameOver == true) { break; }

        // dealing to player
        {
            cout << "Dealing to player" << endl << "-----------------" << endl;

            while (gameOver == false)
            {
                // check the player's score and give advice every iteration (uncomment to enable)
                //provideAdvice(playerCards);

                // allow the player to hit or stand while the game is running
                cout << endl << "Enter h to hit or s to stand:";

                string action;
                cin >> action;

                // prompts user again if invalid input
                while (action != "h" && action != "H" && action != "s" && action != "S")
                {
                    cout << "Invalid input, enter h to hit or s to stand:";
                    cin >> action;
                }

                if (action == "h" || action == "H")
                {
                    // add a card to the player's hand and prints out the cards
                    playerCard = dealCard(deck);
                    addCard(playerCards, playerCard);
                    printCards("+Player+: ", playerCards);
                    cout << "*Dealer*: ";
                    cout << setw(4);
                    cout << dealerCards.pArray[0].name;
                    cout << "  ??" << endl;

                    // check if a blackjack or bust has resulted in an instant w/l
                    gameOver = checkWinOrLose(playerCards, dealerCards, true, result);

                    if (gameOver == true) { break; }
                }

                // leave the while loop and proceed if the user is happy with hand
                else if (action == "s" || action == "S")
                {
                    break;
                }
            }

            // allows the dealer to hit/stand if the game is still going
            if (gameOver == false)
            {
                // print dealer's hidden card
                printCards("+Player+: ", playerCards);
                printCards("*Dealer*: ", dealerCards);

                while (gameOver == false)
                {
                    int dealerScore = getScore(dealerCards);

                    // makes the dealer add cards if current total is below 17
                    if (MAX_SCORE_DEALER_NEED_STAND > dealerScore)
                    {
                        dealerCard = dealCard(deck);
                        addCard(dealerCards, dealerCard);
                        printCards("*Dealer*: ", dealerCards);

                        // check for a blackjack or bust
                        gameOver = checkWinOrLose(playerCards, dealerCards, false, result);

                        if (gameOver == true) { break; }
                    }
                    else
                    {
                        // If noone is bust, check values and give win to highest total
                        result = processNoneGoBustCase(playerCards, dealerCards);
                        gameOver = true;
                    }
                }
            }
        }

    } while (false);

    cout << "---------------------------" << endl;

    // release memory after game is over
    releaseMemory(dealerCards);
    releaseMemory(playerCards);

    return result;
}

// function for playing multiple games
void playGames(CardArray& deck)
{
    // keep track of wins and losses
    int wins = 0;
    int losses = 0;
    int draws = 0;

    int result;

    bool keepPlay = true;

    // user's input
    string action;

    cout << "BLACKJACK" << endl;
    cout << "---------" << endl;

    while (keepPlay == true)
    {
        // ask for user input, prompt message depends if it's the first game
        if (wins + losses + draws == 0)
        {
            cout << "Do you want to play a hand of blackjack (y to play)? Press any key to exit: ";
        }
        else
        {
            cout << "Do you want to play another hand (y to play)? Press any key to exit: ";
        }

        cin >> action;

        if (action == "y" || action == "Y")
        {
            // run a round of blackjack and tally results
            result = blackjack(deck);

            if (result == PLAYERWIN) { wins++; }
            else if (result == TIE) { draws++; }
            else if (result == DEALERWIN) { losses++; }
        }
        else
        {
            keepPlay = false;
        }
    }

    // output final scores once game is over
    cout << endl;
    cout << "Thanks for playing, you played " << (wins + losses + draws) << " games and your record was:" << endl;
    cout << "  wins: " << wins << endl;
    cout << "losses: " << losses << endl;
    cout << " draws: " << draws << endl;
}


int main()
{
    CardArray cards;

    getNewDeck(cards);

    cout << "Print the deck" << endl;
    printDeck(cards);

    shuffleDeck(cards);

    cout << "Print the shuffled deck" << endl;
    printDeck(cards);

    playGames(cards);

    // release memory
    releaseMemory(cards);
}
