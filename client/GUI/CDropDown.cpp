#include "CGUI.h"

CDropDown::CDropDown( CGUI *Gui, int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetElement( Gui, X, Y, Width, Height, String, String2, Callback );

	m_bDropped = false;
	m_iSelected = m_iMouseOverIndex = 0;
	iEdge = iButton = 0;

	SetThemeElement( pGui->GetThemeElement( "DropDown" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "DropDown", 0 );
	else
		SetElementState( "Norm" );
}

void CDropDown::Draw()
{
	CPos Pos = *GetRelPos();
	if(GetParent()) Pos = *GetParent()->GetAbsPos() + Pos;

	SElementState * pState = GetElementState();

	if( pState )
	{
		pLeft->Draw( Pos, iEdge, GetHeight() );
		pButton->Draw( Pos.GetX()+GetWidth()-iButton, Pos.GetY(), iButton, GetHeight() );
		pMiddle->Draw( Pos.GetX()+iEdge-1, Pos.GetY(), GetWidth()-iEdge-iButton+2, GetHeight() );

		GetFont()->DrawString( Pos.GetX() + iPadding, Pos.GetY() + GetHeight() / 2, FT_VCENTER, pString, m_vEntrys[ m_iSelected ].m_sString.c_str() );
		
		if( m_bDropped && m_vEntrys.size() )
		{
			D3DVIEWPORT9 vp;
			pGui->GetDevice()->GetViewport(&vp);
			if(vp.Height < Pos.GetY()+GetHeight()+18*m_vEntrys.size())
			{
				int up = 18 * m_vEntrys.size();
				pGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY()-up, GetWidth(), up, pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

				for( int iIndex = 0; iIndex < static_cast<int>( m_vEntrys.size() ); iIndex++ )
				{
					if( iIndex == m_iMouseOverIndex )
					{
						pGui->FillArea( Pos.GetX() + 1, Pos.GetY() - up + ( iIndex * 18 ), GetWidth() - 2, 18,pSelectedInner->GetD3DCOLOR() );
						GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() - up + ( iIndex * 18 ) + 8, FT_VCENTER, pSelectedString, m_vEntrys[ iIndex ].m_sString.c_str() );
					}
					else
						GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() - up + ( iIndex * 18 ) + 8, FT_VCENTER, pString, m_vEntrys[ iIndex ].m_sString.c_str() );
				}
			}
			else
			{
				pGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY() + GetHeight(), GetWidth(), 18 * m_vEntrys.size(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

				for( int iIndex = 0; iIndex < static_cast<int>( m_vEntrys.size() ); iIndex++ )
				{
					if( iIndex == m_iMouseOverIndex )
					{
						pGui->FillArea( Pos.GetX() + 1, Pos.GetY() + GetHeight() + ( iIndex * 18 ), GetWidth() - 2, 18,pSelectedInner->GetD3DCOLOR() );
						GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + GetHeight() + ( iIndex * 18 ) + 8, FT_VCENTER, pSelectedString, m_vEntrys[ iIndex ].m_sString.c_str() );
					}
					else
						GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + GetHeight() + ( iIndex * 18 ) + 8, FT_VCENTER, pString, m_vEntrys[ iIndex ].m_sString.c_str() );
				}
			}
		}
	}
}

bool CDropDown::MouseMove( CMouse * pMouse, bool over )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos(), mPos = pGui->GetMouse()->GetPos();

	D3DVIEWPORT9 vp;
	pGui->GetDevice()->GetViewport(&vp);
	int iHeight = 18 * m_vEntrys.size();

	if( m_bDropped )
	{
		if( mPos.GetX() == -1 && mPos.GetY() == -1 )
			pMouse->LoadPos();
	}
	else iHeight = 0;

	bool inArea = over;
	if(vp.Height < (DWORD)Pos.GetY()+GetHeight()+iHeight && m_bDropped)
		inArea &= pMouse->InArea( Pos.GetX(), Pos.GetY()-iHeight, GetWidth(), iHeight + GetHeight() );
	else
		inArea &= pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), iHeight + GetHeight() );
	SetElementState( SetMouseOver( inArea )?"MouseOver":"Norm" );

	if( GetMouseOver() )
	{
		if(vp.Height < (DWORD)Pos.GetY()+GetHeight()+iHeight)
		{
			for( int iIndex = 0; iIndex < static_cast<int>( m_vEntrys.size() ); iIndex++ )
			{
				if( pGui->GetMouse()->InArea( Pos.GetX(), Pos.GetY() - iHeight + ( iIndex * 18 ), GetWidth(), 18 ) )
				{
					m_iMouseOverIndex = iIndex;
					break;
				}
			}
		}
		else
		{
			for( int iIndex = 0; iIndex < static_cast<int>( m_vEntrys.size() ); iIndex++ )
			{
				if( pGui->GetMouse()->InArea( Pos.GetX(), Pos.GetY() + GetHeight() + ( iIndex * 18 ), GetWidth(), 18 ) )
				{
					m_iMouseOverIndex = iIndex;
					break;
				}
			}
		}
	}

	pMouse->SetPos( mPos );
	return inArea;
}

bool CDropDown::KeyEvent( SKey sKey )
{
	if( !sKey.m_vKey )
	{
		if( pGui->GetMouse()->GetLeftButton() )
		{
			if( GetMouseOver() )
			{
				if( m_bDropped )
				{	
					m_iSelected = m_iMouseOverIndex;
					if( GetAction() )
						GetAction()( this, SELECT, m_iSelected );

					m_bDropped = false;
					SetElementState( "Norm" );
				}
				else
				{
					GetParent()->BringToTop( this );

					m_bDropped = true;
					SetElementState( "Pressed" );
				}

				pGui->GetMouse()->SetLeftButton( 0 );
			}
			else
				m_bDropped = false;
		}
	}
	return 0;
}

void CDropDown::AddElement( std::string sElem, std::string sValue )
{
	m_vEntrys.push_back( SEntry( sElem, sValue ) );
}

std::string CDropDown::GetValue()
{
	return m_vEntrys[ m_iSelected ].m_sValue;
}

void CDropDown::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pString = pState->GetColor( "String" );
	pInner = pState->GetColor( "Inner" );
	pBorder = pState->GetColor( "Border" );
	pSelectedInner = pState->GetColor( "SelectedInner" );
	pSelectedString = pState->GetColor( "SelectedString" );

	pButton = pState->GetTexture( "Button" );
	pLeft = pState->GetTexture( "Left" );
	pMiddle = pState->GetTexture( "Middle" );

	SetHeight(pState->GetInt("Height"));
	iEdge = pState->GetInt("SizeEdge");
	iButton = pState->GetInt("SizeButton");
	iPadding = pState->GetInt("Padding");
}