#pragma once

#include "Player.h"
#include "TurnLoop.h"
#include <string>

class Game
{
private:
	Player m_player;

	void ReadRules();
	int GetNumOfPlayers();
	void SetPlayerNames(int numOfPlayers);
	void GameRounds(int numOfPlayers);
public:

	void RunGame();
};