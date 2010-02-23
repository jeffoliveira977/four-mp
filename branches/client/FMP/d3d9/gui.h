#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "../GUI/CGUI.h"

class FMPGUI
{
	bool g_Mouse[3];
	CWindow * StartMenu;
	CWindow * ServerList;
	CWindow * Chat;
	CWindow * Option;
public:
	FMPGUI();

	void Load(IDirect3DDevice9 * g_pDevice);
	void HandleMessage(UINT Msg, WPARAM wParam, LPARAM lParam);
	void MoveMouse(int x, int y);
	void Draw();

	CWindow * GetStartMenu();
	CWindow * GetServerList();
	CWindow * GetChat();
	CWindow * GetOption();
};

extern FMPGUI Gui;