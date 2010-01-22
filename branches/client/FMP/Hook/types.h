/*
*	Created:			26.10.09
*	Created by:			009 & WNeZRos
*	Last Modifed:		-
*/
#pragma once
#include "enums.h"

#define DefineScriptHandle(name) typedef struct : FMPHookHandle {} name

typedef struct _FMPHookHandle
{
	private:
		unsigned int Handle;				

	public:
		_FMPHookHandle() : Handle(0) {}
		_FMPHookHandle(unsigned int handle) : Handle(handle) {}
		inline bool IsNull() { return Handle == 0; }
		inline bool IsValid() { return Handle != 0; }
		inline unsigned int Get() { return Handle; }

		inline bool operator == (const _FMPHookHandle &other) { return Handle == other.Handle; }
		inline bool operator != (const _FMPHookHandle &other) { return Handle != other.Handle; }

		inline bool operator == (const unsigned int handle) { return Handle == handle; }
		inline bool operator != (const unsigned int handle) { return Handle != handle; }
} FMPHookHandle;

DefineScriptHandle(Ped);
DefineScriptHandle(Vehicle);
DefineScriptHandle(Cam);
DefineScriptHandle(CarGenerator);
DefineScriptHandle(Group);
DefineScriptHandle(Train);
DefineScriptHandle(Pickup);
DefineScriptHandle(Object);
DefineScriptHandle(Interior);
DefineScriptHandle(Blip);
DefineScriptHandle(Texture);
DefineScriptHandle(TextureDict);
DefineScriptHandle(CoverPoint);
DefineScriptHandle(DecisionMaker);
DefineScriptHandle(Camera);
DefineScriptHandle(TaskSequence);

typedef unsigned int ScriptVoid;
typedef unsigned int ScriptAny;
typedef unsigned int ScriptHandle;
typedef void * ptr;

template <typename T>
inline T *ptr_cast(unsigned int value)
{
	return *reinterpret_cast<T **>(&value);
}

inline ptr ptr_cast(unsigned int value)
{
	return *reinterpret_cast<ptr*>(&value);
}

template <class T>
inline T* ptr_cast(ptr value)
{
    return reinterpret_cast<T*>(value);
}

typedef struct { float X, Y, Z; } Vector3;
