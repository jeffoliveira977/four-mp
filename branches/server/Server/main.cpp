/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#define DEBUG
#define LOG

#pragma comment (lib ,"squirrel.lib")
#pragma comment (lib ,"sqstdlib.lib")
///////////////////////////////////////////////
//             I N C L U D E S               //
///////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
// BeLKa
#include "sq\squirrel.h"
#include "sq\sqstdaux.h"
#include "sq\sqstdblob.h"
#include "sq\sqstdio.h"
#include "sq\sqstdmath.h"
#include "sq\sqstdstring.h"
#include "sq\sqstdsystem.h"
// RakNet
#include "net\RakNetworkFactory.h"
#include "net\RakPeerInterface.h"
#include "net\MessageIdentifiers.h"
#include "net\BitStream.h"
// Other
#include "main.h"
#include "rpc.h"
#include "sq.h"
#include "manager.h"
#include "cfg.h"

///////////////////////////////////////////////
//                V  A  R  S                 //
///////////////////////////////////////////////
RakPeerInterface *net;
HSQUIRRELVM v; 
Player gPlayer[MAX_PLAYERS];
Vehicle gVehicle[MAX_VEHICLES];
Server sConf;
PlayerClass pClass[MAX_PCLASS];
int LastCheck = 0;

void print(const char *string, ...)
{
	va_list arglist; 
    va_start(arglist, string); 
	vprintf(string, arglist); 
	printf("\n");
	#ifdef LOG
		FILE *f = fopen("server.log", "a");
		SYSTEMTIME time;
		GetSystemTime(&time);
		fprintf(f, "[%02d.%02d %02d:%02d:%02d] ", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond);
		vfprintf(f, string, arglist);
		fprintf(f, "\n");
		fclose(f);
	#endif
    va_end(arglist); 
}

void debug(const char *string, ...)
{
	va_list arglist; 
    va_start(arglist, string); 
	#ifdef DEBUG
		vprintf(string, arglist); 
		printf("\n");
		FILE *f = fopen("server.log", "a");
		SYSTEMTIME time;
		GetSystemTime(&time);
		fprintf(f, "<%02d.%02d %02d:%02d:%02d> ", time.wDay, time.wMonth, time.wHour, time.wMinute, time.wSecond);
		vfprintf(f, string, arglist);
		fprintf(f, "\n");
		fclose(f);
	#endif
    va_end(arglist); 
}

void LoadConfig()
{
	sConf.Run = 1;
	strcpy(sConf.GameMode[0], "blank.nut");
	sConf.Port = 7777;

	CFG *config = new CFG("server.cfg");
	strcpy(sConf.Name, config->GetVara("Name"));
	sConf.Port = config->GetVari("Port");
	strcpy(sConf.GameMode[0], config->GetVara("GameMode0"));
	strcpy(sConf.GameMode[1], config->GetVara("GameMode1"));
	strcpy(sConf.GameMode[2], config->GetVara("GameMode2"));
	strcpy(sConf.GameMode[3], config->GetVara("GameMode3"));
	strcpy(sConf.FilterScripts, config->GetVara("FilterScript")); 
	strcpy(sConf.Lang, config->GetVara("Lang")); 
	strcpy(sConf.Password, config->GetVara("Password")); 
	strcpy(sConf.RconPassword, config->GetVara("RconPassword")); 
	strcpy(sConf.ServerURL, config->GetVara("ServerURL")); 
}

int main()
{
	print("Starting...");
	LoadConfig();

	// Init RakNet
	net = RakNetworkFactory::GetRakPeerInterface();
	SocketDescriptor s(sConf.Port, 0);
	net->SetMaximumIncomingConnections(MAX_PLAYERS);
	net->Startup(MAX_PLAYERS, 1, &s, 1);

	// Register RPC
	REGISTER_STATIC_RPC(net, ClientConnect);
	REGISTER_STATIC_RPC(net, Check);
	REGISTER_STATIC_RPC(net, MovePlayer);
	REGISTER_STATIC_RPC(net, DuckPlayer);
	REGISTER_STATIC_RPC(net, JumpPlayer);

	REGISTER_STATIC_RPC(net, PlayerCancelEnterInVehicle);
	REGISTER_STATIC_RPC(net, PlayerExitFromVehicle);
	REGISTER_STATIC_RPC(net, PlayerEnterInVehicle);

	REGISTER_STATIC_RPC(net, PlayerFire);
	REGISTER_STATIC_RPC(net, PlayerAim);
	REGISTER_STATIC_RPC(net, SwapGun);
	REGISTER_STATIC_RPC(net, PlayerParams);

	REGISTER_STATIC_RPC(net, PlayerSpawn);
	REGISTER_STATIC_RPC(net, Select_ModelChanged);
	REGISTER_STATIC_RPC(net, SyncSkinVariation);

	// Init Squerrel
	v = sq_open(1024); 
    sqstd_seterrorhandlers(v);
    sq_setprintfunc(v, printfunc);

	// Register Script Funcions
	register_global_func(v, (SQFUNCTION)sq_printr, "printr");
	register_global_func(v, (SQFUNCTION)sq_CreateCar, "CreateCar");
	register_global_func(v, (SQFUNCTION)sq_GiveWeapon, "GiveWeapon");
	register_global_func(v, (SQFUNCTION)sq_addPlayerClass, "addPlayerClass");
	register_global_func(v, (SQFUNCTION)sq_enableComponentSelect, "enableComponentSelect");

    sq_pushroottable(v); 

	// Register Standart Script Functions
	sqstd_register_stringlib(v);
    sqstd_register_mathlib(v); 
    sqstd_register_systemlib(v); 
    sqstd_seterrorhandlers(v);

	// Load Game Mode
	char gamemode[256];
	sprintf(gamemode, "gamemodes/%s", sConf.GameMode[0]);
	if(!SQ_SUCCEEDED(sqstd_dofile(v, _SC(gamemode), 0, 1))) 
    {
		print("Can't load gamemode");
		return 1;
    }

	sc_OnGameModeInit(v); // Call OnGameModeInit

	// Body
	Packet *pack;
	debug("Started");
	
	while(sConf.Run == 1)
	{
		pack = net->Receive();
		if(pack)
		{
			debug("Pack: %s[%d], %s", pack->data, pack->data[0], pack->systemAddress.ToString());
			if((pack->data[0] == ID_DISCONNECTION_NOTIFICATION) || (pack->data[0] == ID_CONNECTION_LOST)) // Disconnect
			{
				int index = GetPlayerID(pack->systemAddress);
				print("Player %s[%d] disconnected", gPlayer[GetPlayerID(pack->systemAddress)].name, GetPlayerID(pack->systemAddress));
				PlayerDisconnect(index);
				for(int i=0; i<MAX_PLAYERS;i++)
				{
					if(gPlayer[i].connected == 1)
					{ 
						RakNet::BitStream bsSend;
						bsSend.Write(index);
						net->RPC("Disconnect",&bsSend,HIGH_PRIORITY,RELIABLE,0,gPlayer[i].sa,false, 0, UNASSIGNED_NETWORK_ID, 0);
					}
				}
			}
			else if(pack->data[0] == ID_NEW_INCOMING_CONNECTION)
			{
				debug("New connection from %s:%d", pack->systemAddress.ToString(0), pack->systemAddress.port);
			}
			else if(pack->data[0] == ID_CONNECTION_REQUEST)
			{
				debug("New connection request");
			}
		}
		if(GetTickCount() - LastCheck >= 30000)
		{
			LastCheck = GetTickCount();
			SystemAddress client;

			for(int i = 0; i < MAX_PLAYERS; i++)
			{
				if(gPlayer[i].connected == 1)
				{
					client.SetBinaryAddress(gPlayer[i].ip);
					client.port = gPlayer[i].port;

					RakNet::BitStream bsSend;
					net->RPC("Check",&bsSend,HIGH_PRIORITY,RELIABLE,0,client,false, 0, UNASSIGNED_NETWORK_ID, 0);
					debug("Check %d", i);
				}
			}
		}
		net->DeallocatePacket(pack);
		Sleep(100);
	}

	sc_OnGameModeExit(v); // Call OnGameModeExit
	return 0;
}