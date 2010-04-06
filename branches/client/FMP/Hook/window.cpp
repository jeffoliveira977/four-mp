#include <windows.h>
#include "../main.h"
#include "window.h"
#include "../chat.h"
#include "../Hook/classes.h"
#include "../d3d9/Gui.h"

extern FMPHook HOOK;
WNDPROC gameProc;

LRESULT DefWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (clientstate.input)
	{
	case InputStateGame:
		{
		return CallWindowProc(gameProc, hWnd, Msg, wParam, lParam);
		break;
		}
	case InputStateChat:
		{
			if (Msg == WM_KEYUP)
			{
				if(enterChat == -1 && wParam == 192)
				{
					enterChat = 0;
					HOOK.InputFreeze(0);
				}
				else if(wParam == 13 && enterChat != -1)
				{
					if(strlen(enterMsg) != 0)
						SendChatMessage();
					else
					{	
						enterChat = -1;
						enterMsg[0] = 0;
					}
					HOOK.InputFreeze(1);
				}
				else if(wParam == 8)
				{
					enterChat--;
					enterMsg[enterChat] = 0;
				}
				else if(enterChat != -1)
				{
					if(enterChat != 255)
					{
						BYTE ks[256];
						GetKeyboardState(ks);
						ToAsciiEx(wParam,0,ks, (LPWORD)&enterMsg[enterChat],0, GetKeyboardLayout(GetCurrentThreadId()));
						if(enterMsg[enterChat] < 32) enterChat--;
						enterChat++;
						enterMsg[enterChat] = 0;
					}
				}
			}
			break;
		}
	case InputStateGui:
		{
			Gui.HandleMessage(Msg, wParam, lParam);
			break;
		}
	}
	//return DefWindowProc(hWnd, Msg, wParam, lParam);
	return CallWindowProc(gameProc, hWnd, Msg, wParam, lParam);
}