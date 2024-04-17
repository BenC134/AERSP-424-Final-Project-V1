// AERSP 424 Final Project
// Ben Cutuli and Nicholas Alfree
// Blackjack Simulation

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <memory>
#include <regex>
#include <thread>
#include <mutex>
#include "function.h"

// Global variables
std::string intro = "Welcome to Blackjack!"; // Introduction message
float min_bet = 10.00; // Minimum bet amount

// Main function
int main()
{
    std::cout << intro << std::endl;
    std::cout << "To play at this table, the minimum bet is $" << min_bet << "." << std::endl;
    char startGame;
    std::cout << "Would you like to play? (y/n): ";
    std::cin >> startGame;
    if (startGame == 'y' || startGame == 'Y')
    {
        std::srand(std::time(0)); // Seed random number generator
        double startingMoney = 1000; // Starting money for the player
        unsigned int numDecks = 6; // Number of decks to use in the game
        playBlackjack(startingMoney, numDecks); // Start the Blackjack game
    }
    else
    {
        std::cout << "Maybe next time! Goodbye." << std::endl;
    }
    return 0;
}