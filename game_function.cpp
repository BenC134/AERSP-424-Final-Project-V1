#include "function.h"
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <memory>
#include <regex>

// Enumeration for card ranks and suits
class Card
{
public:
    enum Rank { Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King }; // Rank enumeration
    enum Suit { Clubs, Diamonds, Hearts, Spades }; // Suit enumeration

    // Constructor for Card class
    Card(Rank r, Suit s) : rank(r), suit(s) {}

    // Function to get the value of a card
    int getValue() const
    {
        if (rank >= Jack && rank <= King)
        {
            return 10;
        }
        return rank;
    }

    // Function to convert card to string representation
    std::string toString() const
    {
        std::string rankStr;
        if (rank >= Two && rank <= Ten)
        {
            rankStr = std::to_string(rank);
        }
        else
        {
            switch (rank)
            {
            case Ace:
                rankStr = "A";
                break;
            case Jack:
                rankStr = "J";
                break;
            case Queen:
                rankStr = "Q";
                break;
            case King:
                rankStr = "K";
                break;
            }
        }
        std::string suitStr;
        switch (suit)
        {
        case Clubs:
            suitStr = "-C";
            break;
        case Diamonds:
            suitStr = "-D";
            break;
        case Hearts:
            suitStr = "-H";
            break;
        case Spades:
            suitStr = "-S";
            break;
        }
        return rankStr + suitStr;
    }

    // Overload the < operator to compare cards based on rank
    bool operator<(const Card& other) const
    {
        return rank < other.rank;
    }

private:
    Rank rank; // Rank of the card
    Suit suit; // Suit of the card
};

// Template class for a deck of cards
template<typename CardType>
class Deck
{
public:
    // Constructor for Deck class
    Deck(int numDecks)
    {
        // Populate the deck with cards from multiple decks
        for (int i = 0; i < numDecks; ++i)
        {
            for (int j = Card::Ace; j <= Card::King; ++j)
            {
                for (int k = Card::Clubs; k <= Card::Spades; ++k)
                {
                    cards.push_back(Card(static_cast<Card::Rank>(j), static_cast<Card::Suit>(k)));
                }
            }
        }
    }

    // Function to shuffle the deck
    void shuffle()
    {
        std::random_shuffle(cards.begin(), cards.end());
    }

    // Function to draw a card from the deck
    Card& draw()
    {
        Card& drawnCard = cards.back();
        cards.pop_back();
        return drawnCard;
    }

private:
    std::deque<Card> cards; // Deck of cards stored in a deque container
};

// Class representing a hand of cards
class Hand
{
public:
    // Function to add a card to the hand using integer representation
    void addCard(int rank, int suit)
    {
        Card newCard(static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit));
        cards.push_back(newCard);
    }

    // Function to add a card to the hand using Card object
    void addCard(const Card& card)
    {
        cards.push_back(card);
    }

    // Calculates total values of cards in hand
    int getTotal() const
    {
        int total = 0;
        bool hasAce = false;
        for (const auto& card : cards)
        {
            total += card.getValue();
            if (card.getValue() == 1)
            {
                hasAce = true;
            }
        }
        if (hasAce && total + 10 <= 21)
        {
            total += 10;
        }
        return total;
    }

    // Function to display the cards in the hand
    void display() const
    {
        for (const auto& card : cards)
        {
            std::cout << card.toString() << " ";
        }
        std::cout << std::endl;
    }

    // Function to get the string representation of the first card in the hand
    std::string getVisibleCard() const
    {
        if (!cards.empty())
        {
            return cards[0].toString();
        }
        return "";
    }

    // Virtual function to check if the hand has a Blackjack
    virtual bool isBlackjack() const
    {
        return cards.size() == 2 && getTotal() == 21;
    }

    // Virtual function to check if the hand is busted (total value exceeds 21)
    virtual bool isBusted() const
    {
        return getTotal() > 21;
    }

    // Virtual function to retrieve the amount of bet placed on the hand
    virtual double getBetAmount() const
    {
        // Default implementation returns 0
        return 0.0;
    }

    // Function to sort the cards in the hand
    void sortCards()
    {
        std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b)
            {
                return a.getValue() < b.getValue();
            });
    }

private:
    std::vector<Card> cards; // Cards in the hand stored in a vector container
};

// Class representing a player's hand in Blackjack
class PlayerHand : public Hand
{
private:
    double betAmount; // Store the bet amount for the player hand
public:
    // Override isBlackjack to include specific conditions for PlayerHand
    bool isBlackjack() const override
    {
        // Check if the hand has a Blackjack according to PlayerHand rules
        return Hand::isBlackjack(); // For simplicity, PlayerHand follows the same rules as Hand
    }
    // Override getBetAmount to return the bet amount for the player hand
    double getBetAmount() const override
    {
        return betAmount;
    }
    // Function to set the bet amount for the player hand
    void setBetAmount(double amount)
    {
        betAmount = amount;
    }
};

// Class representing a dealer's hand in Blackjack
class DealerHand : public Hand
{
private:
    double betAmount; // Store the bet amount for the dealer hand
public:
    // Override isBlackjack to include specific conditions for DealerHand
    bool isBlackjack() const override
    {
        // Check if the hand has a Blackjack according to DealerHand rules
        return Hand::isBlackjack(); // For simplicity, DealerHand follows the same rules as Hand
    }
    // Override getBetAmount to return the bet amount for the dealer hand
    double getBetAmount() const override
    {
        return betAmount;
    }
    // Function to set the bet amount for the dealer hand
    void setBetAmount(double amount)
    {
        betAmount = amount;
    }
};

// Class for managing positive messages during the game
class PositiveMessages
{
private:
    std::unique_ptr<std::vector<std::string>> messages; // Vector of positive messages
    int consecutiveWins; // Count of consecutive wins

public:
    // Constructor for PositiveMessages class
    PositiveMessages() : messages(std::make_unique<std::vector<std::string>>()), consecutiveWins(0)
    {
        // Initialize positive messages
        messages->push_back("You're on a winning streak!");
        messages->push_back("Keep it up! You're doing great!");
        messages->push_back("Nice job! You're unbeatable!");
    }

    // Function to update consecutive wins and messages
    void update(bool win)
    {
        if (win)
        {
            consecutiveWins++;
            if (consecutiveWins >= messages->size())
                consecutiveWins = messages->size() - 1; // Avoid index out of bounds
        }
        else
        {
            consecutiveWins = 0; // Reset consecutive wins if the player loses
        }
    }

    // Function to get a positive message based on consecutive wins
    std::string getMessage() const
    {
        return (*messages)[consecutiveWins];
    }
};

// Function to validate user input for bet amount using regular expressions
bool isValidBet(const std::string& input)
{
    // Regular expression pattern to match a positive floating-point number
    std::regex pattern("^\\d+(\\.\\d+)?$");

    // Check if the input matches the pattern
    return std::regex_match(input, pattern);
}

// Function to play Blackjack game
void playBlackjack(double money, unsigned int numDecks)
{
    Deck<Card> deck(6); // Create a deck of cards with 6 decks
    deck.shuffle(); // Shuffle the deck

    double bet; // Bet amount
    std::string betInput; // Input for bet amount
    PositiveMessages positiveMessages; // Object for managing positive messages

    while (money > 0)
    {
        std::cout << "\nMoney available: $" << money << std::endl;
        std::cout << "Enter your bet: ";
        std::cin >> betInput;

        if (!isValidBet(betInput))
        {
            std::cout << "Invalid bet amount. Please enter a valid number." << std::endl;
            continue;
        }

        bet = std::stod(betInput);

        if (bet > money)
        {
            std::cout << "You don't have enough money to bet that amount!" << std::endl;
            continue;
        }
        if (bet < 10)
        {
            std::cout << "Minimum bet is $10!" << std::endl;
            continue;
        }
        std::cout << "\n######################################################### \n\n";

        Hand playerHand, dealerHand; // Create hands for player and dealer
        playerHand.addCard(deck.draw()); // Deal two cards to player
        playerHand.addCard(deck.draw());
        dealerHand.addCard(deck.draw()); // Deal two cards to dealer
        dealerHand.addCard(deck.draw());

        // Sort the player's hand
        playerHand.sortCards();

        std::cout << "Your hand: ";
        playerHand.display();
        std::cout << "Your total: " << playerHand.getTotal() << std::endl;
        std::cout << "Dealer's hand: " << dealerHand.getVisibleCard() << " ?" << std::endl;

        if (playerHand.isBlackjack())
        {
            std::cout << "Congratulations! You got a Blackjack!" << std::endl;
            positiveMessages.update(true);
            std::cout << positiveMessages.getMessage() << std::endl; // Display positive message
            money += bet * 1.5; // Blackjack payout
            continue; // Proceed to next round
        }

        char choice;
        while (true)
        {
            std::cout << "\nDo you want to hit (h) or stand (s)? ";
            std::cin >> choice;
            if (choice == 'h')
            {
                playerHand.addCard(deck.draw());
                std::cout << "Your hand: ";
                playerHand.display();
                std::cout << "Your total: " << playerHand.getTotal() << std::endl;
                if (playerHand.isBusted())
                {
                    std::cout << "Busted! You lose." << std::endl;
                    positiveMessages.update(false); // Reset consecutive wins
                    money -= bet;
                    break;
                }
            }
            else if (choice == 's')
            {
                break;
            }
            else
            {
                std::cout << "Invalid choice. Please enter 'h' or 's'." << std::endl;
            }
        }

        if (playerHand.getTotal() <= 21)
        {
            std::cout << "Dealer's hand: ";
            dealerHand.display();
            std::cout << "Dealer total: " << dealerHand.getTotal() << std::endl;
            while (dealerHand.getTotal() < 17)
            {
                dealerHand.addCard(deck.draw());
                std::cout << "Dealer hits: ";
                dealerHand.display();
                std::cout << "Dealer total: " << dealerHand.getTotal() << std::endl;
            }
            if (dealerHand.isBusted() || playerHand.getTotal() > dealerHand.getTotal())
            {
                std::cout << "You win!" << std::endl;
                positiveMessages.update(true); // Increment consecutive wins
                std::cout << positiveMessages.getMessage() << std::endl; // Display positive message
                money += bet;
            }
            else if (playerHand.getTotal() < dealerHand.getTotal())
            {
                std::cout << "Dealer wins." << std::endl;
                positiveMessages.update(false); // Reset consecutive wins
                money -= bet;
            }
            else
            {
                std::cout << "It's a tie. Bet returned." << std::endl;
            }
        }
    }
    std::cout << "You are out of money. Game over!" << std::endl;
}