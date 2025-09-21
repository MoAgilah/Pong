#include "GameMode.h"

#include "../GameObjects/Player.h"

GameType GameMode::s_type = VsAI;
PlayerIdentifiers GameMode::s_player1 = Unset;
PlayerIdentifiers GameMode::s_player2 = Unset;

void GameMode::AssigPlayer1(PlayerIdentifiers id)
{
	s_player1 = id;
}

void GameMode::AssigPlayer2(PlayerIdentifiers id)
{
	s_player2 = id;
}

void GameMode::AssigPlayers(PlayerIdentifiers ply1, PlayerIdentifiers ply2)
{
	s_player1 = ply1;
	s_player2 = ply2;
}

bool GameMode::ArePlayersAssigned()
{
	switch (s_type)
	{
	case VsAI:
	case vsWall:
		if (s_player1 == Unset)
			return false;

		s_player2 = GetOpposite(s_player1);

		return true;
	case VsPly:
	case AIvsAI:
		if (s_player1 == Unset || s_player2 == Unset)
			return false;

		if (s_player1 == s_player2)
			return false;

		return true;
	default:
		return false;
	}
}

PlayerIdentifiers GameMode::GetOpposite(PlayerIdentifiers id)
{
	if (id == Player1)
		return Player2;

	if (id == Player2)
		return Player1;

	return Unset;
}
