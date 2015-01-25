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
//#include <windows.h>
#include "prehdr.h"
#include <windowsx.h>
#include "resource.h"
#include "keyhook.h"
#include "encode.h"
#include "about.h"
#include "keydlg.h"
#include "mainwnd.h"
#include "userpref.h"

// external variable in mainwnd.cpp
extern TCHAR AppTitle[100];
extern CMainWnd MainWnd;
extern CUserPref UserPref;

#define VN_EXPAND_BUTTON _TEXT("Më réng")
#define VN_CONTRACT_BUTTON _TEXT("Thu nhá")

#define EN_EXPAND_BUTTON _TEXT("Expand")
#define EN_CONTRACT_BUTTON _TEXT("Compact")

DlgItemInfo VnDlgItems[] = {
    {IDC_TELEX,				_T("KiÓu TELEX")},
    {IDC_VNI,				_T("KiÓu VNI")},
    {IDC_FREE_MARKING,		_T("Cho phÐp gâ dÊu mò, mãc ë cuèi tõ")},
    {IDC_TONE_CTRL,			_T("Ph¶i gâ dÊu ngay sau nguyªn ©m")},
    {IDC_MODERN_STYLE,		_T("Bá dÊu oµ, uý (thay v× ßa, óy)")},
	{IDC_VIET_GUI,			_T("Vietnamese interface")},
    {IDC_SHOW_DLG,			_T("BËt héi tho¹i nµy khi khëi ®éng")},
    {IDC_RESET,				_T("MÆc ®Þnh")},
    {IDC_EXIT,				_T("&KÕt thóc")},
    {IDOK,					_T("&Taskbar")},
    {IDC_OPEN_HELP,			_T("&H­íng dÉn")},
    {IDC_ABOUT,				_T("Th«ng tin")},
    {IDC_CONTROL_GROUP,		_T("§iÒu khiÓn")},
    {IDC_CHARSET_PROMPT,	_T("B¶ng m·:")},
    {IDC_SWITCHKEY_PROMPT,	_T("PhÝm chuyÓn:")},
    {IDC_KEYMODE_PROMPT,	_T("KiÔu gâ phÝm:")},
    {IDC_OPTION_GROUP,		_T("Lùa chän")},
	{IDC_AUTO_START,		_T("Khëi ®éng cïng Windows")}};


DlgItemInfo EnDlgItems[] = {
    {IDC_TELEX,				_T("TELEX Typing")},
    {IDC_VNI,				_T("VNI Typing")},
    {IDC_FREE_MARKING,		_T("Allow typing ^, v, + ... at the end")},
    {IDC_TONE_CTRL,			_T("Tone mark next to vowel")},
    {IDC_MODERN_STYLE,		_T("Use oa`, uy' (instead of o`a, u'y)")},
	{IDC_VIET_GUI,			_T("Giao dien tieng Viet")},
    {IDC_SHOW_DLG,			_T("Show this dialog box at startup")},
    {IDC_RESET,				_T("Default")},
    {IDC_EXIT,				_T("&Exit")},
    {IDOK,					_T("&Taskbar")},
    {IDC_OPEN_HELP,			_T("&Help")},
    {IDC_ABOUT,				_T("About")},
    {IDC_CONTROL_GROUP,		_T("Settings")},
    {IDC_CHARSET_PROMPT,	_T("Character set:")},
    {IDC_SWITCHKEY_PROMPT,	_T("Switch key:")},
    {IDC_KEYMODE_PROMPT,	_T("Input method:")},
    {IDC_OPTION_GROUP,		_T("Advanced options")},
	{IDC_AUTO_START,		_T("Auto-run UniKey at boot time")}};

//------------------------------------------
int CKeyDlg::init(HINSTANCE hInst, LPCSTR lpszTemplate, 
			    HWND hWndOwner)
{
	return CExpandDialog::init(
				hInst,
				lpszTemplate,
				hWndOwner,
				IDC_COMPACT_FRAME,
				IDC_EXPAND,
				IDC_FOOTER_FRAME,
				(UserPref.m_vietGUI)? VN_EXPAND_BUTTON : EN_EXPAND_BUTTON,
				(UserPref.m_vietGUI)? VN_CONTRACT_BUTTON : EN_CONTRACT_BUTTON);
}

//------------------------------------------
void CKeyDlg::loadDlgInfo(CUserPref *pPref)
{
	int i;
	SendMessage(m_codeList, CB_RESETCONTENT, 0, 0);
	for (i=0; i < TOTAL_CHARSET; i++)
		SendMessage(m_codeList, CB_ADDSTRING, 0, (LPARAM)CharsetTable[i].name);
	SendMessage(m_codeList, CB_SETCURSEL, pPref->m_codeTable, 0); 
	SendMessage(m_showCtrl,
				BM_SETCHECK,
				(pPref->m_showDlg)? BST_CHECKED : BST_UNCHECKED,
				0);
	SendMessage(m_markCheck,
				BM_SETCHECK,
				(pPref->m_freeMarking)? BST_CHECKED : BST_UNCHECKED,
				0);
	SendMessage(m_toneCtrl,
				BM_SETCHECK,
				(pPref->m_toneNextToVowel)? BST_CHECKED : BST_UNCHECKED,
				0);

	SendMessage(m_modernStyleCheck,
				BM_SETCHECK,
				(pPref->m_modernStyle)? BST_CHECKED : BST_UNCHECKED,
				0);

	SendMessage(m_swKeyBtn1, 
				BM_SETCHECK, 
				(pPref->m_switchKey==0)? BST_CHECKED : BST_UNCHECKED, 
				0);

	SendMessage(m_swKeyBtn2, 
				BM_SETCHECK, 
				(pPref->m_switchKey==1)? BST_CHECKED : BST_UNCHECKED, 
				0);
	SendMessage(m_telexBtn,
				BM_SETCHECK, 
				(pPref->m_inMethod == TELEX_INPUT)? BST_CHECKED : BST_UNCHECKED,
				0);

	SendMessage(m_vniBtn,
				BM_SETCHECK, 
				(pPref->m_inMethod == VNI_INPUT)? BST_CHECKED : BST_UNCHECKED,
				0);
				
	SendDlgItemMessage(m_hWnd, IDC_VIET_GUI, BM_SETCHECK,
				(pPref->m_vietGUI)? BST_CHECKED : BST_UNCHECKED,
				0);

	SendDlgItemMessage(m_hWnd, IDC_AUTO_START, BM_SETCHECK,
				(pPref->m_autoStart)? BST_CHECKED : BST_UNCHECKED,
				0);

	if (!MainWnd.isVietFontOK())
		EnableWindow(GetDlgItem(m_hWnd, IDC_VIET_GUI), FALSE);

	pPref->m_changes = 0;
}

//------------------------------------------
void CKeyDlg::storeDlgInfo(CUserPref *pPref)
{
	int sel;
	// input method: VNI or TELEX
	if (SendMessage(m_telexBtn, BM_GETCHECK, 0, 0) == BST_CHECKED)
		sel = TELEX_INPUT;
	else
		sel = VNI_INPUT;
	if (sel != (int)pPref->m_inMethod) {
		pPref->m_changes |= PREF_CHANGE_METHOD;
		pPref->m_inMethod = sel;
	}

	// character set
	sel = (int)SendMessage(m_codeList,CB_GETCURSEL,0,0);
	if (sel != (int)pPref->m_codeTable) {
		pPref->m_changes |= PREF_CHANGE_CHARSET;
		pPref->m_oldTable = pPref->m_codeTable;
		pPref->m_codeTable = sel;
		if (sel != (int)pPref->m_otherCharset && sel >= POPULAR_CHARSETS) {
			pPref->m_changes |= PREF_CHANGE_OTHERCHARSET;
			pPref->m_otherCharset = sel;
		}
	}
	sel = (SendMessage(m_swKeyBtn1, BM_GETCHECK, 0, 0) == BST_CHECKED)? 0 : 1;
	if (sel != (int)pPref->m_switchKey) {
		pPref->m_changes |= PREF_CHANGE_SWITCHKEY;
		pPref->m_switchKey = sel;
	}

	sel = (SendMessage(m_markCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)? 1 : 0;
	if (sel && !pPref->m_freeMarking || !sel && pPref->m_freeMarking) {
		pPref->m_changes |= PREF_CHANGE_OPTIONS;
		pPref->m_freeMarking = sel;
	}

	sel = (SendMessage(m_toneCtrl, BM_GETCHECK, 0, 0) == BST_CHECKED)? 1 : 0;
	if (sel && !pPref->m_toneNextToVowel || !sel && pPref->m_toneNextToVowel) {
		pPref->m_changes |= PREF_CHANGE_OPTIONS;
		pPref->m_toneNextToVowel = sel;
	}

	sel = (SendMessage(m_modernStyleCheck, BM_GETCHECK, 0, 0) == BST_CHECKED)? 1 : 0;
	if (sel && !pPref->m_modernStyle || !sel && pPref->m_modernStyle) {
		pPref->m_changes |= PREF_CHANGE_OPTIONS;
		pPref->m_modernStyle = sel;
	}

	sel = (SendMessage(m_showCtrl,BM_GETCHECK,0,0) == BST_CHECKED) ? 1 : 0;
	if (sel && !pPref->m_showDlg || !sel && pPref->m_showDlg) {
		pPref->m_changes |= PREF_CHANGE_SHOWDLG;
		pPref->m_showDlg = sel;
	}
	
	// GUI interface
	if (SendDlgItemMessage(m_hWnd, IDC_VIET_GUI,BM_GETCHECK, 0, 0) == BST_CHECKED)
		sel = 1;
	else
		sel = 0;
	if (sel && !pPref->m_vietGUI || !sel && pPref->m_vietGUI) {
		pPref->m_changes |= PREF_CHANGE_GUI;
		pPref->m_vietGUI = sel;
	}

	if (SendDlgItemMessage(m_hWnd, IDC_AUTO_START, BM_GETCHECK, 0, 0) == BST_CHECKED)
		sel = 1;
	else
		sel = 0;
	if (sel && !pPref->m_autoStart || !sel && pPref->m_autoStart) {
		pPref->m_changes |= PREF_CHANGE_AUTOSTART;
		pPref->m_autoStart = sel;
	}

}

//------------------------------------------
BOOL CKeyDlg::DialogProc(
					HWND hDlg,
					UINT uMsg,
					WPARAM wParam,
					LPARAM lParam)
{
	switch (uMsg) {
	case WM_SYSCOMMAND:
		{
			WORD cmd = (wParam & 0xFFF0) >> 4;
			switch (cmd) {
			case IDM_ABOUT:
				DoAbout(m_hInst, hDlg,
					(SendDlgItemMessage(m_hWnd, IDC_VIET_GUI, BM_GETCHECK, 0, 0) == BST_CHECKED)? 1 : 0);
				return TRUE;
			}
		}
		break;
	case WM_DRAWITEM:
		// Send reflection message to back the control
		return SendDlgItemMessage(hDlg, wParam, WM_DRAWITEM, wParam, lParam);
	case WM_DESTROY:
		if (m_hStdFont != NULL) {
			DeleteObject(m_hStdFont);
			m_hStdFont = NULL;
		}
		break;
	}
	return CExpandDialog::DialogProc(hDlg, uMsg, wParam, lParam);
}

//------------------------------------------
BOOL CKeyDlg::onInitDialog()
{
	if (!CExpandDialog::onInitDialog())	return FALSE;

//	setDlgFont(UserPref.m_vietGUI ? 1 : 0);
	if (MainWnd.isVietFontOK())
		setDlgFont(1);
	setItemsText(UserPref.m_vietGUI? 1 : 0);

	expand(FALSE);
	//SetForegroundWindow(m_hWnd);
	HICON mainBigIcon, mainSmallIcon;
	SetWindowText(m_hWnd, AppTitle);
	// Setup dialog box icons
	mainBigIcon = (HICON)LoadImage(m_hInst, 
									MAKEINTRESOURCE(IDR_MAINFRAME), 
									IMAGE_ICON,
									32, 32, 
									LR_SHARED);
	mainSmallIcon = (HICON)LoadImage(m_hInst, 
									MAKEINTRESOURCE(IDR_MAINFRAME), 
									IMAGE_ICON,
									16, 16, 
									LR_SHARED);
	SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)mainBigIcon);
	SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)mainSmallIcon);

	// Setup system menu
	HMENU hSysMenu = GetSystemMenu(m_hWnd, FALSE);
	if (hSysMenu) {
		TCHAR itemStr[100];
		LoadString(m_hInst, IDS_ABOUT, itemStr, sizeof(itemStr)/sizeof(TCHAR));
		AppendMenu(hSysMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hSysMenu, MF_STRING, ((UINT)IDM_ABOUT)<<4, itemStr);
	}
	// Setup owner-draw button
	m_aboutBtn.init(m_hInst, m_hWnd, GetDlgItem(m_hWnd, IDC_ABOUT));
	m_aboutBtn.setIcon(IDR_MAINFRAME, 16, 16);

	m_okBtn.init(m_hInst, m_hWnd, GetDlgItem(m_hWnd, IDOK));
	m_okBtn.setIcon(IDI_OK, 16, 16);
	m_okBtn.setDefaultButton(TRUE);
	SendMessage(m_hWnd, DM_SETDEFID, IDOK, 0);

	m_cancelBtn.init(m_hInst, m_hWnd, GetDlgItem(m_hWnd, IDC_EXIT));
	m_cancelBtn.setIcon(IDI_CANCEL, 16, 16);

	m_helpBtn.init(m_hInst, m_hWnd, GetDlgItem(m_hWnd, IDC_OPEN_HELP));
	m_helpBtn.setIcon(IDI_HELP, 16, 16);

	m_resetBtn.init(m_hInst, m_hWnd, GetDlgItem(m_hWnd, IDC_RESET));
	m_resetBtn.setIcon(IDI_RESET, 16, 16);

	m_expandBtn.init(m_hInst, m_hWnd, GetDlgItem(m_hWnd, IDC_EXPAND));
	m_expandBtn.setIcon(IDI_EXPAND, 16, 16);

	m_codeList = GetDlgItem(m_hWnd,IDC_CODE_TABLE);
	m_showCtrl = GetDlgItem(m_hWnd,IDC_SHOW_DLG);
	m_markCheck = GetDlgItem(m_hWnd, IDC_FREE_MARKING);
	m_toneCtrl = GetDlgItem(m_hWnd, IDC_TONE_CTRL);
	m_modernStyleCheck = GetDlgItem(m_hWnd, IDC_MODERN_STYLE);

	m_swKeyBtn1 = GetDlgItem(m_hWnd, IDC_SW_KEY1);
	m_swKeyBtn2 = GetDlgItem(m_hWnd, IDC_SW_KEY2);

	m_telexBtn = GetDlgItem(m_hWnd, IDC_TELEX);
	m_vniBtn = GetDlgItem(m_hWnd, IDC_VNI);

	loadDlgInfo(m_pPref);
	return TRUE;
}

//-----------------------------------------------
BOOL CKeyDlg::onOK()
{
	HWND hFocusWnd = GetFocus();
	if (IsPushButton(hFocusWnd)) {
		int id = GetDlgCtrlID(hFocusWnd);
		if (id != IDOK) {
			SendMessage(hFocusWnd, BM_CLICK, 0, 0);
			return FALSE;
		}
	}
	storeDlgInfo(m_pPref);
	return TRUE;
}


//-----------------------------------------------
BOOL CKeyDlg::onCancel()
{
	ModifyStatusIcon();
	return TRUE;
}

//-----------------------------------------------
BOOL CKeyDlg::onCommand(WPARAM wParam, LPARAM lParam)
{
	WORD cmd = GET_WM_COMMAND_ID(wParam, lParam);
	switch (cmd) {
	case IDC_ABOUT:
		DoAbout(m_hInst, m_hWnd, 
			(SendDlgItemMessage(m_hWnd, IDC_VIET_GUI, BM_GETCHECK, 0, 0) == BST_CHECKED)? 1 : 0);
		return TRUE;
	case IDC_OPEN_HELP:
		MainWnd.openHelp();
		return TRUE;
	case IDC_RESET:
		{
			CUserPref current, def;
			storeDlgInfo(&current);
			def.setDefault(&current);
			loadDlgInfo(&def);
		}
		break;
	case IDC_EXIT:
		storeDlgInfo(m_pPref);
		EndDialog(m_hWnd, IDC_EXIT);
		return TRUE;
	case IDC_VIET_GUI:
		if (GET_WM_COMMAND_CMD(wParam, lParam) == BN_CLICKED) {
			int langID;
			langID = (SendDlgItemMessage(m_hWnd, IDC_VIET_GUI, BM_GETCHECK, 0, 0) == BST_CHECKED)? 1 : 0;
			setItemsText(langID);
		}
		break;
	}
	return CExpandDialog::onCommand(wParam, lParam);
}

//----------------------------------------------
BOOL CKeyDlg::onDialogExpanding(BOOL bExpanded)
{
	CExpandDialog::onDialogExpanding(bExpanded);
	m_expandBtn.setIcon(bExpanded ? IDI_EXPAND : IDI_CONTRACT, 16, 16);
	return TRUE;
}


//----------------------------------------------
// set text labels according to the chosen GUI language
// langID = 0 -> English (default)
//----------------------------------------------
void CKeyDlg::setItemsText(int langID)
{
	int items, i;
	DlgItemInfo *texts;
	if (langID == 0) {
		texts = EnDlgItems;
		items = sizeof(EnDlgItems)/sizeof(DlgItemInfo);
		_tcscpy(m_strExpand, EN_EXPAND_BUTTON);
		_tcscpy(m_strContract, EN_CONTRACT_BUTTON);
	}
	else {
		texts = VnDlgItems;
		items = sizeof(VnDlgItems)/sizeof(DlgItemInfo);
		_tcscpy(m_strExpand, VN_EXPAND_BUTTON);
		_tcscpy(m_strContract, VN_CONTRACT_BUTTON);
	}
	for (i=0; i<items; i++)
		SendDlgItemMessage(m_hWnd, texts[i].id, WM_SETTEXT, 0, (LPARAM)texts[i].text);
	SendDlgItemMessage(m_hWnd, m_nIDButton, WM_SETTEXT, 
		0, (LPARAM)(m_bExpanded ? m_strContract : m_strExpand));
}

//----------------------------------------------
// set font for dialog items accoring to the selected GUI language
// langID = 0 -> English (default)
//----------------------------------------------
void CKeyDlg::setDlgFont(int langID)
{
	if (m_hStdFont != NULL)
		DeleteObject(m_hStdFont);
	HDC hDC = GetDC(m_hWnd);
	if (langID == 0)
		m_hStdFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	else
		m_hStdFont = CustomCreateFont(hDC, VIET_FONT_NAME, VIET_FONT_SIZE);
	SendMessage(m_hWnd, WM_SETFONT, (WPARAM)m_hStdFont, TRUE);
	HWND wndChild = GetTopWindow(m_hWnd);
	for ( ; wndChild != NULL; wndChild = GetWindow(wndChild, GW_HWNDNEXT))
		SendMessage(wndChild, WM_SETFONT, (WPARAM)m_hStdFont, TRUE);
	ReleaseDC(m_hWnd, hDC);
}
