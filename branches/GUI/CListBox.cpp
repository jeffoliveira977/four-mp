#include "CListBox.h"

CListBox::CListBox( int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetElement( X, Y, Width, Height, String, String2, Callback );
	m_iMouseOverIndex = -1;
	m_iMouseSelect = -1;

	pSlider = new CHelperSlider( CPos( GetWidth() - HELPERSLIDER_WIDTH + 2, 0 ), GetHeight() );

	SetThemeElement( gpGui->GetThemeElement( "ListBox" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "ListBox", 0 );
	else
		SetElementState( "Norm" );
}

void CListBox::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	gpGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	int iAddHeight = gpGui->GetFont()->GetStringHeight();
	if( m_vRows.size() )
		for( int i = pSlider->GetValue(), iHeight = 0; i < static_cast<int>( m_vRows.size() ) && iHeight < GetHeight() - gpGui->GetFont()->GetStringHeight(); i++ )
		{
			CColor * pColor = 0;

			if( i == m_iMouseOverIndex && GetMouseOver() )
				pColor = pMouseOverString;
			else
				pColor = pString;

			if(m_iMouseSelect == i)	
			{
				gpGui->DrawOutlinedBox( Pos.GetX() + 2, Pos.GetY() + iHeight, GetWidth() - HELPERSLIDER_WIDTH, iAddHeight, pString->GetD3DCOLOR(), pString->GetD3DCOLOR() );
				pColor = pInner;
			}

			gpGui->GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + iHeight, 0, pColor, m_vRows[ i ].c_str(), GetWidth() - HELPERSLIDER_WIDTH );
			iHeight += iAddHeight;
		}

	pSlider->Draw( Pos );
}	

void CListBox::PreDraw()
{
	pSlider->PreDraw();
}

void CListBox::MouseMove( CMouse * pMouse )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	SetMouseOver( pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) );

	m_iMouseOverIndex = -1;
	for( int i = pSlider->GetValue(), iHeight = 0, iStringHeight = gpGui->GetFont()->GetStringHeight(); i < static_cast<int>( m_vRows.size() ) || iHeight < GetHeight(); i++ )
	{
		if( pMouse->InArea( Pos.GetX(), Pos.GetY() + iHeight, GetWidth() - BUTTON_HEIGHT, iStringHeight ) )
			m_iMouseOverIndex = i;

		iHeight += iStringHeight;
	}

	pSlider->MouseMove( Pos, pMouse );
}

void CListBox::KeyEvent( SKey sKey )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	if( !sKey.m_vKey )
	{
		if( GetMouseOver() )
		{
			if(m_iMouseSelect > -2) m_iMouseSelect = m_iMouseOverIndex;
			if( m_iMouseOverIndex >= 0 && GetAction() && gpGui->GetMouse()->GetLeftButton() )
				GetAction()( reinterpret_cast<char*>( m_iMouseOverIndex ), this );
		}
	}

	if( GetMouseOver() || ( !sKey.m_bDown && !gpGui->GetMouse()->GetWheel() )  )
	{
		pSlider->KeyEvent( Pos, sKey );
		MouseMove( gpGui->GetMouse() );
	}
}

int CListBox::GetSize()
{
	return m_vRows.size();
}

void CListBox::PutStr( std::string sString, int Index )
{
	if(Index >= static_cast<int>( m_vRows.size() ) || Index == -1)
	{
		pSlider->SetMaxValue( m_vRows.size() );
		m_vRows.push_back(sString);
	}
	else
	{
		m_vRows[Index] = sString;
	}
}

std::string CListBox::GetRow( int iIndex )
{
	if( iIndex >= 0 && iIndex < static_cast<int>( m_vRows.size() ) )
		return m_vRows[ iIndex ];
	return std::string();
}

void CListBox::Clear()
{
	m_vRows.clear();
}

void CListBox::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pInner = pState->GetColor( "Inner" );
	pBorder = pState->GetColor( "Border" );
	pString = pState->GetColor( "String" );
	pMouseOverString = pState->GetColor( "MouseOverString" );
}

void CListBox::ShowSlider( bool bShow )
{
	pSlider->Show = bShow;
}

int CListBox::GetSelected()
{
	return m_iMouseSelect;
}

void CListBox::SetSelect(int Item)
{
	m_iMouseSelect = Item;
}