#pragma once

enum PlayerIdentifiers;

enum GameType
{
	NotSet = -1,
	VsAI,
	VsPly,
	AIvsAI,
	vsWall
};

class GameMode
{
public:
	static void AssigPlayer1(PlayerIdentifiers id);
	static void AssigPlayer2(PlayerIdentifiers id);
	static PlayerIdentifiers GetOpposite(PlayerIdentifiers id);
	static void AssigPlayers(PlayerIdentifiers ply1, PlayerIdentifiers ply2);
	static bool ArePlayersAssigned();

	static GameType s_type;
	static PlayerIdentifiers s_player1;
	static PlayerIdentifiers s_player2;
};