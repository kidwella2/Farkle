#pragma once

#include <string>
#include <vector>
using StringVector = std::vector<std::string>;
using IntVector = std::vector<int>;

class Player
{
private:
	StringVector m_name;
	IntVector m_score;

public:
	Player()
	{
	}

	//Getter and setter method for player name
	void SetName(const std::string& name)
	{
		m_name.push_back(name);
		m_score.push_back(0);
	}

	const std::string& GetName(int playerNum) const
	{
		return m_name[playerNum];
	}

	//Getter and setter fuction for player score
	void SetScore(const int score, int playerNum)
	{
		m_score[playerNum] = score;
	}

	const int GetScore(int playerNum) const
	{
		return m_score[playerNum];
	}
};