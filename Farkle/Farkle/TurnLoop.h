#pragma once

#include "Player.h"
#include <string>
#include <array>
const unsigned int ARRAY_SIZE = 6;
using DiceArray = std::array<int, ARRAY_SIZE>;

class Turn
{
private:
	Player m_player;

	DiceArray GetPointDice(DiceArray dice);
	DiceArray NonPointTriple(DiceArray dice);
	int CalculatePoints(DiceArray dice);
public:

	int PlayerTurn(std::string player, int score);
};