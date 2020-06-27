#include "GameLoop.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

//Function used for players turn
int Turn::PlayerTurn(string player, int score)
{
    //Define variables
    unsigned int numOfPointDice = 0, numChosen = 0;
    unsigned int tempScore = 0, roundScore = 0, countTriple = 0;
    string chosen;
    DiceArray diceSet, pointDice, findTriple;
    DiceArray chosenDice = { 0, 0, 0, 0, 0, 0 };
    char endTurn;
    bool turnOver = false;

    //Generate random numbers using the current time
    srand(time(NULL));

    cout << endl << player << "'s turn" << endl;

    //Loop until an action that ends a turn occurs
    while (!turnOver) {
        //Assign and display the 6 dice variables as random integers (1-6)
        for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
            if (chosenDice[i] == 0) {
                diceSet[i] = (rand() % 6) + 1;
            }
            cout << "Dice " << i + 1 << ": " << diceSet[i] << endl;
        }

        //Get point dice
        pointDice = GetPointDice(diceSet);

        //Loop to display point dice
        for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
            if (pointDice[i] != -1 && pointDice[i] != chosenDice[i]) {
                cout << "Point Dice " << i + 1 << ": " << pointDice[i] << endl;
                ++numOfPointDice;
            }
        }

        //Calculate temp score and reroll if all point dice
        if (numOfPointDice == 6 || numChosen == 6) {
            tempScore += CalculatePoints(diceSet);
            cout << endl << "Temp score: " << tempScore << endl << endl;
            numOfPointDice = 0;
            numChosen = 0;
            for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
                pointDice[i] = 0;
                chosenDice[i] = 0;
            }
        }
        //Farkle and end turn if no new point dice
        else if (numOfPointDice == numChosen) {
            cout << "Farkle!" << endl;
            tempScore = 0;
            turnOver = true;
        }
        else {
            //Prompt user to enter point dice to set aside
            cout << endl << "Choose point dice to set aside (1's, 5's, and triples)" << endl;
            cout << "Enter the corresponding dice numbers (1-6)" << endl;
            cin >> chosen;

            //Get non 1 or 5 triples
            findTriple = NonPointTriple(diceSet);

            //Loop through player input to get chosen dice
            try {
                for (string::iterator iter = chosen.begin(); iter != chosen.end(); ++iter) {
                    //Temp changes *iter to value - 1 with adjusted characters from switching from string
                    int temp = *iter - 49;
                    if (temp >= 0 && temp <= 5) {
                        if (pointDice[temp] != -1) {
                            //Find if only part of a triple is called
                            if (findTriple[temp] == 1) {
                                findTriple[temp] = -1;
                                ++countTriple;
                            }
                            chosenDice[temp] = pointDice[temp];
                            ++numChosen;
                        }
                    }
                }
            }
            //Throw exception for invalid input
            catch (const out_of_range&)
            {
                cout << endl << "The input wasn't recognized!" << endl;
            }

            //Put rest of a triple in chosenDice is only part was selected
            for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
                if (findTriple[i] == 1 && countTriple != 0 &&
                    countTriple != 3) {
                    chosenDice[i] = pointDice[i];
                    ++numChosen;
                    ++countTriple;
                }
            }

            //Chooses all point dice if user selects no point dice
            if (numChosen == 0 && numOfPointDice != 0) {
                for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
                    if (pointDice[i] != -1) {
                        chosenDice[i] = pointDice[i];
                    }
                }
            }

            //Displays chosen dice
            for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
                if (chosenDice[i] != 0) {
                    cout << "Chosen dice " << (i + 1) << ": " << chosenDice[i] << endl << endl;
                }
            }
            //Calculate and display temp score
            roundScore = CalculatePoints(chosenDice);
            cout << "Temp score: " << tempScore + roundScore << endl << endl;

            //Get option to quit turn and start collecting points after 1000 temp score
            if (score == 0 && tempScore + roundScore >= 1000) {
                cout << "Would you like to continue? (y/n)" << endl;
                cin >> endTurn;
                if (endTurn == 'n') {
                    tempScore += roundScore;
                    turnOver = true;
                }
            }
            //Get option to quit and keep points after score is over 1000
            else if (score >= 1000) {
                cout << "Would you like to continue? (y/n)" << endl;
                cin >> endTurn;
                if (endTurn == 'n') {
                    tempScore += roundScore;
                    turnOver = true;
                }
            }
        }
    }

    return score + tempScore;
}

//Function used to get the point dice
DiceArray Turn::GetPointDice(DiceArray dice)
{
    //Define variables
    int tripleValue = -1, countTriple = 0;
    DiceArray pointDice, countDice;
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        pointDice[i] = dice[i];
        countDice[i] = 0;
    }

    //Loop to count number of each face value
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        countDice[(pointDice[i]) - 1] += 1;
    }

    //Loop for finding triples
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {

        //Return all dice if they all have the same value
        if (countDice[i] == 6) {
            return pointDice;
        }
        //If first triple, store face value of triple to locate later
        else if (countDice[i] >= 3 && tripleValue == -1) {
            tripleValue = (i + 1);
        }
        //Return all dice if 2 sets of triples
        else if (countDice[i] == 3) {
            return pointDice;
        }
    }

    //Loop for finding point dice with a triple
    if (tripleValue != -1) {
        for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
            if (tripleValue == pointDice[i] && countTriple < 3) {
                ++countTriple;
            }
            else if (tripleValue == pointDice[i]) {
                if (pointDice[i] != 1 && pointDice[i] != 5) {
                    pointDice[i] = -1;
                }
            }
            else if (pointDice[i] != 1 && pointDice[i] != 5) {
                pointDice[i] = -1;
            }
        }
    }
    //Loop for finding point dice without a triple
    else {
        for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
            if (pointDice[i] != 1 && pointDice[i] != 5) {
                pointDice[i] = -1;
            }
        }
    }


    return pointDice;
}

//Function to locate triple not with 1's or 5's
DiceArray Turn::NonPointTriple(DiceArray dice)
{
    //Define variables
    int tripleValue = -1, countTriple = 0;
    DiceArray tripleDice, countDice;
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        tripleDice[i] = dice[i];
        countDice[i] = 0;
    }

    //Loop to count number of each face value
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        countDice[(tripleDice[i]) - 1] += 1;
    }

    //Loop for finding triples
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        if (countDice[i] >= 3 && i != 0 && i != 4) {
            tripleValue = (i + 1);
        }
    }

    //Set values for locating triples (1 is yes, -1 is no)
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        if (tripleValue != -1 && tripleDice[i] == tripleValue &&
            countTriple < 3) {
            tripleDice[i] = 1;
            ++countTriple;
        }
        else if (tripleValue != -1 && tripleDice[i] == tripleValue) {
            tripleDice[i] = -1;
        }
        else {
            tripleDice[i] = -1;
        }
    }

    return tripleDice;
}

int Turn::CalculatePoints(DiceArray dice)
{
    //Define variables
    int tripleValue = -1, score = 0;
    DiceArray countDice;

    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        countDice[i] = 0;
    }

    //Loop to count number of each face value if not 0
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {
        if (dice[i] != 0) {
            countDice[dice[i] - 1] += 1;
        }
    }

    //Loop for finding triples
    for (unsigned int i = 0; i < ARRAY_SIZE; ++i) {

        //Calculate 2 triples if they all have the same value
        if (countDice[i] == 6) {
            if ((i + 1) == 1) {
                score = 2 * 1000;
                return score;
            }
            else {
                score = 2 * (i + 1) * 100;
                return score;
            }
        }
        //Calculate triple, store face value of triple to locate later
        else if (countDice[i] >= 3 && tripleValue == -1) {
            tripleValue = i + 1;
            if (tripleValue == 1) {
                score = 1000;
            }
            else {
                score = (i + 1) * 100;
            }
        }
        //Calculate all dice if 2 sets of triples
        else if (countDice[i] == 3) {
            score += ((i + 1) * 100);
            return score;
        }
    }

    //Calculate point dice with a triple
    if (tripleValue != -1) {
        countDice[tripleValue - 1] -= 3;
        score += ((countDice[0] * 100) + (countDice[4] * 50));
        return score;
    }
    //Calculate point dice without a triple
    else {
        score = (countDice[0] * 100) + (countDice[4] * 50);
        return score;
    }

    return score;
}