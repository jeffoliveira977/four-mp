#pragma once

#include "CGUI.h"

class CGUI;

class CLine
{
	CGUI *gpGui;
	CColor *color;
	int sX, sY, eX, eY, Size;
	CWindow *wParent;
public:
	CLine( CGUI *Gui, int sX, int sY, int eX, int eY, int size, CColor *color, CWindow *eP = NULL);
	void Draw();
};

class CBox
{
	CGUI *gpGui;
	CColor * pInner, * pBorder;
	int X, Y, Width, Height;
	CWindow *wParent;
public:
	CBox( CGUI *Gui, int iX, int iY, int iWidth, int iHeight, CColor *In, CColor *Bor, CWindow *eP = NULL);
	void Draw();
};

class CImage
{
	CGUI *gpGui;
	int X, Y, Width, Height;
	CTexture * pImg;
	CWindow *wParent;
public:
	CImage( CGUI *Gui, int iX, int iY, int iWidth, int iHeight, CTexture *iTxt, CWindow *eP = NULL);
	void Draw();
};

