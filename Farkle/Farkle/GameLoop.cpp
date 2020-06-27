// farkle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/* Name: Austin Kidwell
Course: IT 312
Date: 6/19/20
Assignment Name: 7-1 Dice Game (Farkle)

Description: This program reads in a text file containing the rules for farkle and displays
it to the user. Next, it obtains the number of people who would like to play. It uses that
information to create vectors for the player's names and their score. The game rounds then
start and loops through the rounds until a player reaches 10,000 points and there is a winner.
The game gives everyone a turn in order followed by the current scores at the end of each round.

In a turn, it starts by rolling the dice along with displaying the roll with the point dice
specified. If all 6 dice are score dice, the score will be calculated as a temporary score and
the user would reroll. If no new point dice are rolled, it's a farkle and you turn ends with
the user getting no points for the round. Next, the user is prompted to choose what point dice
they want to set aside. The point dice are 1's, 5's, and triples. The program keeps track of if
the triple isn't made with 1's or 5's so if the user only chooses part of the triple the rest
would be automatically chosen too. If the user only chooses non point dice, all the point dice
would be selected as chosen. The chosen dice are displayed along with the temporary score from
from the turn. If the player's score is 0, they have to wait till their temporary score is over
1,000 points before they have the option to end their turn and keep their points. If the player's
score is over 1,000 points, they have the option to quit their turn and keep their points at the
end of each round. The player's turn is over after they either roll a farkle (no new point dice)
or they choose to end their turn and keep their points.


Issues: I couldn't figure out how to get the GetNumOfPlayers() function to catch a string input from
breaking the loop. If characters for dice chosen are random letters instead of numbers 1-6, the point
dice will sometimes show improper results.
*/

#include "GameLoop.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
using namespace std;

void Game::RunGame()
{
    //Define variables
    int numOfPlayers;

    //Call to read rules
    ReadRules();

    //Get number of players
    numOfPlayers = GetNumOfPlayers();  

    //Set player names
    SetPlayerNames(numOfPlayers);

    //Loop through game rounds until someone wins
    GameRounds(numOfPlayers);
}

//Function to read farkle rules
void Game::ReadRules()
{
    //Define variable
    string line;

    //Stream used to read from the entered text file
    ifstream rulesFile("it312_farkle_rules.txt");

    //Runs only if the text file is found and open
    if (rulesFile.is_open()) {
        //Loops through the text file line-by-line and displays results
        while (getline(rulesFile, line)) {
            cout << line << endl;
        }
        rulesFile.close();
    }
    //Prints out error message if file wasn't found/open
    else cout << "The specified text file wasn't found";
}

int Game::GetNumOfPlayers()
{
    //Define variables
    int numOfPlayers = 0;
    double intCheck = 0;

    try {
        //Get user input for number of players
        while (numOfPlayers < 2 || numOfPlayers > 99 || intCheck != numOfPlayers) {
            cout << endl << "How many players are in the game?" << endl;
            cin >> intCheck;
            numOfPlayers = intCheck;
        }
    }
    //Throw exception for invalid input
    catch (const invalid_argument& e)
    {
        cout << endl << "The input wasn't recognized!" << endl;
    }
    return numOfPlayers;
}
//Function for setting player names and storing them in an array
void Game::SetPlayerNames(int numOfPlayers)
{
    for (unsigned int i = 0; i < numOfPlayers; ++i) {
        string name;
        cout << endl << "Enter the player's name: " << endl;
        cin >> name;
        m_player.SetName(name);
        m_player.SetScore(0, i);
    }
}

//Function for game rounds
void Game::GameRounds(int numOfPlayers)
{
    bool gameOver = false;

    //Loop through player turns until someone wins
    while (!gameOver) {
        for (unsigned int i = 0; i < numOfPlayers; ++i) {
            Turn turn;
            int tempScore;
            tempScore = turn.PlayerTurn(m_player.GetName(i), m_player.GetScore(i));
            m_player.SetScore(tempScore, i);
        }
        //Display scores after rounds
        for (unsigned int i = 0; i < numOfPlayers; ++i) {
            cout << endl << m_player.GetName(i) << ": " << m_player.GetScore(i) << endl;
            if (m_player.GetScore(i) >= 10000) {
                gameOver = true;
                cout << endl << "Congratulations " << m_player.GetName(i) << " on winning!" << endl;
            }
        }
    }
}