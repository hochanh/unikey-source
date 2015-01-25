/*------------------------------------------------------------------------------
UniKey - Vietnamese Keyboard for Windows
Copyright (C) 1998-2001 Pham Kim Long
Contact: longp@cslab.felk.cvut.cz

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
--------------------------------------------------------------------------------*/
#ifndef __MAIN_WINDOW_H
#define __MAIN_WINDOW_H

#include "util.h"
#include "userPref.h"
#include "odmenu.h"

#define MAIN_WINDOW_CLASS_NAME _TEXT("UniKey MainWnd")
#define VIET_FONT_NAME _TEXT("ABC Sans Serif")
#define VIET_FONT_FILE _TEXT("abcserif.fon")
#define VIET_FONT_SIZE 8

class CMainWnd : public CVirWindow
{
protected:
//	HMENU m_menu, m_subMenu;
	CODMenu m_menu;
	int m_menuItems;
	TCHAR m_helpPath[MAX_PATH];
	int m_fontLoaded;
//	int m_vietGUI; // vietnamese user interface
	int m_vietFontOK;
public:
	CMainWnd() 
	{
		//m_menu = m_subMenu = NULL;
		m_menuItems = 0;
		m_helpPath[0] = 0;
		m_fontLoaded = 0;
//		m_vietGUI = 0;
		m_vietFontOK = 0;
	};
	
/*
	~CMainWnd()
	{
		if (m_menu)
			DestroyMenu(m_menu);
	}
*/
	BOOL initInstance(HINSTANCE hInstance);
	void initMenuState(const CUserPref & pref);
	virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void cleanup();
	int  loadVietFont();
	void openHelp()
	{
		WinHelp(m_hWnd, m_helpPath, HELP_FINDER, 0);
	}

	int isVietFontOK()
	{
		return m_vietFontOK;
	}
};

HFONT CustomCreateFont(HDC hDC, LPCSTR face, int size);

// Dialog resource
struct DlgItemInfo {
	UINT id;
	TCHAR *text;
};


#endif
