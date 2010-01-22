/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "sq\squirrel.h"
#include "sq\sqstdsystem.h"

// SQ Functions
SQInteger register_global_func(HSQUIRRELVM v,SQFUNCTION f,const char *fname);
void printfunc(HSQUIRRELVM v, const SQChar *s, ...);
void sq_printr(HSQUIRRELVM v);
// SQ Callbacks
void sc_OnGameModeInit(HSQUIRRELVM v);
void sc_OnGameModeExit(HSQUIRRELVM v);
int sc_OnPlayerConnect(HSQUIRRELVM v, int playerid, char name[32]);
void sc_OnPlayerDisconnect(HSQUIRRELVM v, int playerid);
// SQ Script Functions
void sq_CreateCar(HSQUIRRELVM v);
void sq_GiveWeapon(HSQUIRRELVM v);
void sq_addPlayerClass(HSQUIRRELVM v);
void sq_enableComponentSelect(HSQUIRRELVM v);