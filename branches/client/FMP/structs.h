/*
*	Created:			29.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#pragma once
#include "Hook/types.h"
#include "Hook/scripting.h"

struct FPlayer
{
	bool LocalPlayer;
	Ped PedID() 
	{ 
		if(this->LocalPlayer)
			Natives::GetPlayerChar(Natives::ConvertIntToPlayerIndex(Natives::GetPlayerId()), &this->iPedID);
		return this->iPedID;
	}

	Ped iPedID;
	bool connected;
	char ip[16];
	unsigned short port;
	char name[32];
	int model;
	float position[3];
	float angle;
	int last_active;
	int sync_state;
	int currentweapon;
	int weapons[8];
	int ammo[8];
	char animation[128];
	short vehicleindex;
	int seat_id;
	int score;
	int health, armour;
	int wanted_level;
	bool edSprint, edLockon, edDoDriveBy, edUseCover, edConrol, edFreeze; // enable / disable
	bool Aim;
	bool isducking;
	int room;
	unsigned char color[4];
	int compT[11], compD[11];
};

struct PlayerDamage
{
	int pid;
	int hp;
	int armour;
};

struct PlayerClass
{
	int model;
	float x, y, z;
	float angle;
	int gun[3], ammo[3];
};

struct SpawnInfo
{
	int model;
	float x, y, z, r;
	int room;
	int CompT[11], CompD[11];
	int health, armour;
};

struct FVehicle
{
	Vehicle CarID;
	bool exist;
	int model;
	float position[3];
	float angle;
	short Health, engHealth;
	int DoorState[6];
	int DoorLock[6];
	unsigned char color[2];
};

struct FConfig
{
	char server[32];
	int port;
	char Name[32];

	bool ComponentSelect;
	int NumSkins;
	bool SkinSelect;
};

class COLOR
{
public:
	COLOR() {  }
	COLOR(int r, int g, int b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	int r, g, b;
};

struct CHATMSG 
{
	char msg[128];
	COLOR color;
};

/// \brief Indicates the global state of the game.
enum GameState
{
	GameStateOffline, ///< Indicates that client is offline.
	GameStateConnecting, ///< Indicates that client is connecting to the server.
	GameStateInGame, ///< Indicates that client is connected to the server and is playing.
	GameStateSkinSelect, ///< Indicates that client is connected to the server and is choosing their skin.
	GameStateComponentSelect ///< Indicates that client is connected to the server and is choosing their components (clothes).
};

/// \brief Indicates the target that all keyboard and mouse input is redirected to.
enum InputState
{
	InputStateGame, ///< Indicates that all input is going into the game itself.
	InputStateChat, ///< Indicates that all input is going into the chat.
	InputStateGui ///< Indicates that all input is going into the GUI.
};

/// \brief Indicates the global state of the client.
struct ClientState
{
	GameState game; ///< Holds the current game state.
	InputState input; ///< Holds the current input target.
};