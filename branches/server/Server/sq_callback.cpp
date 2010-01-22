/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

#include "sq\squirrel.h"
#include "sq\sqstdsystem.h"
#include "main.h"

extern HSQUIRRELVM v;

void sc_OnGameModeInit(HSQUIRRELVM v)
{
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnGameModeInit"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_call(v,1,0,0); 
	}
	sq_settop(v,top); 
}

void sc_OnGameModeExit(HSQUIRRELVM v)
{

}

int sc_OnPlayerConnect(HSQUIRRELVM v, int playerid, char name[32])
{
	int result;
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnPlayerConnect"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_pushinteger(v,playerid); 
		sq_pushstring(v,name,-1);
		sq_call(v,3,1,0); 
		sq_getinteger(v, sq_gettop(v), &result);
	}
	sq_settop(v,top); 
	return result;
}

void sc_OnPlayerDisconnect(HSQUIRRELVM v, int playerid)
{
	int result;
	int top = sq_gettop(v); 
	sq_pushroottable(v);
	sq_pushstring(v,_SC("OnPlayerConnect"),-1);
	if(SQ_SUCCEEDED(sq_get(v,-2))) { 
		sq_pushroottable(v); 
		sq_pushinteger(v,playerid); 
		sq_call(v,3,0,0); 
	}
	sq_settop(v,top); 
}