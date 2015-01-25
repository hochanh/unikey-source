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
#include "prehdr.h"
#include "mainwnd.h"
#include "resource.h"
#include "keyhook.h"
#include "encode.h"
#include "keydlg.h"
#include "userpref.h"
#include "font.h"

#define WM_MYICON_NOTIFY	(WM_USER+1)
//#define WM_KEY_NOTIFY		(WM_USER+2) // used for short-cut keys

//#define WM_MULTI_INSTANCE (WM_USER+2)

TCHAR *VnMenuText[] = {
		"KÕt thóc",
		"H­íng dÉn",
		"B¶ng ®iÒu khiÓn...",
		"Gâ kiÓu TELEX",
		"Gâ kiÓu VNI",
		"TCVN3 (ABC)",
		"Unicode",
		"VIQR",
		"Other"};
/* //VISCII strings
TCHAR *VnMenuText[] = {
		"Kªt thúc",
		"Hß¾ng dçn",
		"Hµp ði«u khi¬n...",
		"Gõ ki¬u TELEX",
		"Gõ ki¬u VNI",
		"TCVN3 (ABC)",
		"Unicode",
		"VIQR",
		"Other"};
*/
/*
TCHAR *EnMenuText[] = {
		"Exit",
		"Help",
		"Configuration...",
		"TELEX Typing",
		"VNI Typing",
		"TCVN3 (ABC)",
		"Unicode",
		"VIQR",
		"Other"};
*/

/* //VISCII strings
#define VN_EXPAND_BUTTON _TEXT("M· rµng")
#define VN_CONTRACT_BUTTON _TEXT("Thu nhö")
*/

// Global variables
HINSTANCE AppInstance, HookDll;

HHOOK hKeyHook,hMouseHook;
TCHAR AppTitle[100];
CMainWnd MainWnd;

SharedMem ShMem;

BOOL initDLL(HWND hWnd);

CUserPref UserPref; // user preference

BOOL CMainWnd::initInstance(HINSTANCE hInstance)
{
	HWND hWnd = Create(
		            WS_EX_TOOLWINDOW,
					MAIN_WINDOW_CLASS_NAME,
					AppTitle,
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT, 
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					NULL,
					NULL,
					hInstance);
	if (!hWnd)
		return FALSE;
	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);

	m_vietFontOK = 0;
	if (FontExists(VIET_FONT_NAME)) {
		m_vietFontOK = 1;
	}
	else {
		if (loadVietFont()) {
			m_vietFontOK = 1;
		}
	}

	if (UserPref.m_vietGUI && !m_vietFontOK)
		UserPref.m_vietGUI = 0;

	if (UserPref.m_vietGUI) {
		m_menu.initMenu(m_hInst, m_hWnd, IDR_KEY_MENU, VnMenuText);
		m_menu.setFont(VIET_FONT_NAME, VIET_FONT_SIZE);
	}
	else {
		m_menu.initMenu(m_hInst, m_hWnd, IDR_KEY_MENU); // Use menu text from the resource
		m_menu.setFont("", 0); // use Windows standard font
	}

	// Locate help file. The name of help file is "UniKey.hlp", it is supposed to be 
	// in the same location with the exe file.
	GetModuleFileName(NULL, m_helpPath, sizeof(m_helpPath));
	TCHAR *p = _tcsrchr(m_helpPath, _TEXT('\\'));
	_tcscpy(++p, _TEXT("UniKey.hlp"));

	return TRUE;
}

#define FIRST_KEY_MENU_INDEX 8 // This must be changed when menu resource is changed

//-------------------------------------------------
void CMainWnd::initMenuState(const CUserPref & pref)
{
	int sel;
	m_menu.setMenuItemText(CharsetTable[UserPref.m_otherCharset].name, ID_KEY_OTHER, FALSE);
	SetMenuDefaultItem(m_menu.m_hMenu, ID_KEY_PANEL, FALSE);
	if (UserPref.m_codeTable > POPULAR_CHARSETS)
		sel = FIRST_KEY_MENU_INDEX + POPULAR_CHARSETS;
	else
		sel = FIRST_KEY_MENU_INDEX + UserPref.m_codeTable;

	CheckMenuItem(
		m_menu.m_hMenu,
		sel,
		MF_BYPOSITION | MF_CHECKED);

	CheckMenuItem(
		m_menu.m_hMenu,
		(UserPref.m_inMethod == TELEX_INPUT)? ID_TELEX_TYPING : ID_VNI_TYPING,
		MF_BYCOMMAND | MF_CHECKED);
}


//-------------------------------------------------
LRESULT CMainWnd::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int sel;
	LRESULT result;
	result = FALSE;
	static int dlgShown = 0;
	switch (uMsg) {
	/*
	case WM_MULTI_INSTANCE:
		MessageBox(m_hWnd, "UniKey is already running", "Information", MB_ICONINFORMATION);
		result = TRUE;
		break;
	*/
	case WM_MEASUREITEM:
		m_menu.measureItem((LPMEASUREITEMSTRUCT)lParam);
		result = TRUE;
		break;
	case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT lpd = (LPDRAWITEMSTRUCT)lParam;
			if (lpd->CtlType == ODT_MENU)
				m_menu.drawItem(lpd);
		}
		result = TRUE;
		break;
	case WM_MYICON_NOTIFY:
		switch (lParam) {
		case WM_LBUTTONDOWN:
			SwitchMode();
			break;
		case WM_RBUTTONDOWN:
			{
				POINT point;
				GetCursorPos(&point);
				SetForegroundWindow(m_hWnd);
				m_menu.trackPopupMenu(
					TPM_RIGHTALIGN | TPM_BOTTOMALIGN,
					point.x,
					point.y);
				PostMessage(m_hWnd, WM_NULL, 0, 0);
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				UINT id = GetMenuDefaultItem(m_menu.m_hMenu, FALSE, 0);
				if (id != -1) {
					SwitchMode();
					PostMessage(m_hWnd, WM_COMMAND, id, 0);
				}
			}
			break;
		}
		result = TRUE;
		break;
	case WM_COMMAND:
		switch (wParam) {
		case ID_OPEN_HELP:
			openHelp();
			result = TRUE;
			break;
		case ID_END_KEY:
			DeleteStatusIcon();
			DestroyWindow(m_hWnd);
			result = FALSE;
			break;
		case ID_KEY_PANEL:
			if (!dlgShown)
			{
				dlgShown = 1;
				// Show dialog
				//DeleteStatusIcon();
				EnableMenuItem(m_menu.m_hMenu, ID_KEY_PANEL, MF_BYCOMMAND | MF_DISABLED);
				CKeyDlg dlg(&UserPref);
				if (UserPref.m_vietGUI) 
					dlg.init(m_hInst, MAKEINTRESOURCE(IDD_KEY_DIALOG_VIET), m_hWnd);
				else
					dlg.init(m_hInst, MAKEINTRESOURCE(IDD_KEY_DIALOG), m_hWnd);

				int ret = dlg.ShowDialog();
				EnableMenuItem(m_menu.m_hMenu, ID_KEY_PANEL, MF_BYCOMMAND | MF_ENABLED);
				//ModifyStatusIcon();
				result = TRUE;
				if (ret == IDOK && UserPref.m_changes) {
					if (UserPref.m_changes & (PREF_CHANGE_CHARSET | PREF_CHANGE_METHOD)) {
						BuildCodeTable(CharsetTable[UserPref.m_codeTable].id, UserPref.m_inMethod, &ShMem.codeTable);
						SetKeyMode(CharsetTable[UserPref.m_codeTable].id, UserPref.m_inMethod, &ShMem.codeTable);
					}
					if (UserPref.m_changes & PREF_CHANGE_SWITCHKEY)
						SetSwitchKey(UserPref.m_switchKey);
					if (UserPref.m_changes & PREF_CHANGE_OPTIONS)
						SetKeyOptions(UserPref.m_freeMarking, UserPref.m_toneNextToVowel, UserPref.m_modernStyle);
					// Update menu
					if (UserPref.m_changes & PREF_CHANGE_OTHERCHARSET)
						m_menu.setMenuItemText(
								CharsetTable[UserPref.m_otherCharset].name,
								ID_KEY_OTHER,
								FALSE);
									
						/*
						ModifyMenu(m_subMenu, 
							ID_KEY_OTHER, 
							MF_BYCOMMAND | MF_STRING, 
							ID_KEY_OTHER,
							CharsetTable[UserPref.m_otherCharset].name);
						*/
					if (UserPref.m_changes & PREF_CHANGE_CHARSET) {
						CheckMenuItem(m_menu.m_hMenu,
							(UserPref.m_oldTable > POPULAR_CHARSETS)? 
							(POPULAR_CHARSETS + FIRST_KEY_MENU_INDEX) : (UserPref.m_oldTable + FIRST_KEY_MENU_INDEX),
							MF_BYPOSITION | MF_UNCHECKED);
						CheckMenuItem(m_menu.m_hMenu,
							(UserPref.m_codeTable > POPULAR_CHARSETS)? 
							(POPULAR_CHARSETS + FIRST_KEY_MENU_INDEX) : (UserPref.m_codeTable + FIRST_KEY_MENU_INDEX),
							MF_BYPOSITION | MF_CHECKED);
					}
					if (UserPref.m_changes & PREF_CHANGE_METHOD) {
						CheckMenuItem(m_menu.m_hMenu, 
							ID_TELEX_TYPING,
							MF_BYCOMMAND | ((UserPref.m_inMethod == TELEX_INPUT)? MF_CHECKED : MF_UNCHECKED));
						CheckMenuItem(m_menu.m_hMenu, 
							ID_VNI_TYPING,
							MF_BYCOMMAND | ((UserPref.m_inMethod == VNI_INPUT)? MF_CHECKED : MF_UNCHECKED));
					}
					if (UserPref.m_changes & PREF_CHANGE_GUI) {
						// Since the menu is owner-draw, we must reload the source
						// to calculate the correct size

						// backup default item
						//UINT defItem = GetMenuDefaultItem(m_menu.m_hMenu, FALSE, 0);

						if (UserPref.m_vietGUI) {
							m_menu.initMenu(m_hInst, m_hWnd, IDR_KEY_MENU, VnMenuText);
							m_menu.setFont(VIET_FONT_NAME, VIET_FONT_SIZE);
						}
						else {
							m_menu.initMenu(m_hInst, m_hWnd, IDR_KEY_MENU);
							m_menu.setFont("", 0);
						}
						//if (defItem != -1)
						//	SetMenuDefaultItem(m_menu.m_hMenu, defItem, FALSE);
						initMenuState(UserPref);
						//m_menu.setMenuItemText(
						//			CharsetTable[UserPref.m_otherCharset].name,
						//			ID_KEY_OTHER,
						//			FALSE);
					}
				}
				else if (ret == IDC_EXIT)
					SendMessage(m_hWnd, WM_COMMAND, ID_END_KEY, 0);
				dlgShown = 0;
			}
			result = TRUE;
			break;
		case ID_TELEX_TYPING:
		case ID_VNI_TYPING:
			sel = (wParam == ID_TELEX_TYPING)? TELEX_INPUT : VNI_INPUT;
			if (sel != (int)UserPref.m_inMethod) {
				CheckMenuItem(m_menu.m_hMenu, wParam, MF_BYCOMMAND | MF_CHECKED);
				CheckMenuItem(
					m_menu.m_hMenu, 
					(UserPref.m_inMethod == TELEX_INPUT)? ID_TELEX_TYPING : ID_VNI_TYPING,
					MF_BYCOMMAND | MF_UNCHECKED);
				UserPref.m_inMethod = sel;
				BuildInputMethod(sel, &ShMem.codeTable);
				SetInputMethod(sel, ShMem.codeTable.DT);
			}
			result = TRUE;
			break;
		default:
			// determine if wParam is a key menu command
			for (sel=FIRST_KEY_MENU_INDEX; sel <= FIRST_KEY_MENU_INDEX + POPULAR_CHARSETS; sel++) {
				if (GetMenuItemID(m_menu.m_hMenu, sel) == wParam) {
					int charSet = sel - FIRST_KEY_MENU_INDEX;
					if (charSet == POPULAR_CHARSETS)
						charSet = UserPref.m_otherCharset;
					if ((int)UserPref.m_codeTable != charSet) {
						CheckMenuItem(
							m_menu.m_hMenu,
							(UserPref.m_codeTable > POPULAR_CHARSETS)? 
								(POPULAR_CHARSETS + FIRST_KEY_MENU_INDEX) : (UserPref.m_codeTable + FIRST_KEY_MENU_INDEX),
							MF_BYPOSITION | MF_UNCHECKED);
						UserPref.m_codeTable = charSet;
						CheckMenuItem(
							m_menu.m_hMenu,
							sel,
							MF_BYPOSITION | MF_CHECKED);
						//SendMessage(m_codeList, CB_SETCURSEL, UserPref.m_codeTable, 0);
						BuildCodeTable(CharsetTable[UserPref.m_codeTable].id, UserPref.m_inMethod, &ShMem.codeTable);
						SetKeyMode(CharsetTable[UserPref.m_codeTable].id, UserPref.m_inMethod, &ShMem.codeTable);
					}
					break;
					result = TRUE;
				}
			}
		}
		break;
	case WM_DESTROY:
	case WM_QUERYENDSESSION:
		UserPref.m_vietnamese = IsVietnamese();
		UserPref.setValue();
		if (uMsg == WM_DESTROY)
			PostQuitMessage(1);
		result = TRUE;
		break;
	default:
		result = DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return result;
}

//---------------------------------------------
BOOL InitApplication(
       HINSTANCE hInstance)
{
	BOOL bOk;
	// The window class for all instances of the main window.
	WNDCLASSEX wcf;

	// Assign the appropriate values for this main frame window class.
	wcf.cbSize        = sizeof(WNDCLASSEX);
	wcf.cbClsExtra    = 0;            // No per-class extra data.
	wcf.cbWndExtra    = 0;            // No per-window extra data.
	wcf.hInstance     = hInstance;    // Application module instance.
	wcf.lpfnWndProc   = GenWindowProc;  // Global Window Procedure (defined in
									  // UTIL.H for all CVirWindows).
	wcf.hCursor       = NULL;			// Load app cursor.
	wcf.hIcon         = (HICON) LoadImage(           // Load small icon.
									hInstance,
									MAKEINTRESOURCE(IDR_MAINFRAME),
									IMAGE_ICON,
									32, 32,
									LR_SHARED);
	wcf.hIconSm       = NULL;
	wcf.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  // Default backgnd color.
	wcf.style         = 0;				// Class style(s).
	wcf.lpszClassName = MAIN_WINDOW_CLASS_NAME; // Class name.
	wcf.lpszMenuName  = NULL;						  // Menu name.

	// Register the window class and return FALSE if unsuccesful.
	bOk = RegisterClassEx(&wcf);

	return (bOk);
}

//-------------------------------------------
int WINAPI WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow
)
{
	MSG msg;
	//-- Check if a previous instance is loaded
	AppInstance = hInstance;
	LoadString(AppInstance,IDS_APP_TITLE,AppTitle,100);
	if (HWND hPrevWnd = FindWindow(MAIN_WINDOW_CLASS_NAME, AppTitle)) { 
		// Bring in the last instance and exit
		BringWindowToTop(hPrevWnd);
		SetForegroundWindow(hPrevWnd);
		PostMessage(hPrevWnd,WM_COMMAND,ID_KEY_PANEL,0); 
		//PostMessage(hPrevWnd, WM_MULTI_INSTANCE, 0, 0); 
		return 0;
	}
	if (!InitApplication(hInstance))
		return 0;
	
	UserPref.getValue();

	if (!MainWnd.initInstance(hInstance))
		return 0;

	// Init DLL
	HookDll = GetVietHookDll();
	if (!initDLL(MainWnd.m_hWnd))
		return 0;


	MainWnd.initMenuState(UserPref);

	ModifyStatusIcon(); // Show icon on the tray
	if (UserPref.m_showDlg)
		PostMessage(MainWnd.m_hWnd, WM_COMMAND, ID_KEY_PANEL, 0);
	else 
		MessageBeep(0xFFFFFFFF);
	while (GetMessage(&msg, (HWND) NULL, 0, 0)) { 
		TranslateMessage(&msg); 
		DispatchMessage(&msg);
    } 

	// the program is finished
	if (hKeyHook != NULL)
		UnhookWindowsHookEx(hKeyHook);
	if (hMouseHook != NULL)
		UnhookWindowsHookEx(hMouseHook);
	MainWnd.cleanup();
	return 1;
}

//--------------------------------------------
BOOL initDLL(HWND hWnd)
{
	HOOKPROC keyProc = (HOOKPROC)GetProcAddress(HookDll, "MyKeyHook");
	HOOKPROC mouseProc = (HOOKPROC)GetProcAddress(HookDll, "MyMouseHook");
	if (keyProc == NULL || mouseProc == NULL) {
		MessageBox(NULL, _TEXT("Failed to find hook procedures"), _TEXT("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	hKeyHook = SetWindowsHookEx(WH_KEYBOARD, keyProc, HookDll, 0);
	hMouseHook = SetWindowsHookEx(WH_MOUSE,mouseProc, HookDll, 0);

	if (hKeyHook == NULL || hMouseHook == NULL) {
		MessageBox(NULL, _TEXT("Failed to initialize the program"), _TEXT("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Init shared memory
	ShMem.hMainDlg = hWnd;
	ShMem.hEnIcon = LoadIcon(AppInstance,MAKEINTRESOURCE(IDI_EN));
	ShMem.hVietIcon = LoadIcon(AppInstance,MAKEINTRESOURCE(IDI_VIET));
	ShMem.iconShown = 0;
	ShMem.keyHook = hKeyHook;
	ShMem.mouseHook = hMouseHook;
	ShMem.keyMode = CharsetTable[UserPref.m_codeTable].id;
	ShMem.iconMsgId = WM_MYICON_NOTIFY;
//	ShMem.keyMsgId = WM_KEY_NOTIFY;
	ShMem.switchKey = UserPref.m_switchKey;
	ShMem.vietKey = UserPref.m_vietnamese;
	ShMem.inMethod = UserPref.m_inMethod;
	ShMem.freeMarking = UserPref.m_freeMarking;
	ShMem.toneNextToVowel = UserPref.m_toneNextToVowel;
	ShMem.modernStyle = UserPref.m_modernStyle;

	BuildCodeTable(CharsetTable[UserPref.m_codeTable].id, UserPref.m_inMethod, &ShMem.codeTable);
	InitSharedMem(&ShMem);
	return TRUE;
}


void getFontFile(TCHAR *path, int size)
{
	GetModuleFileName(NULL, path, size);
	TCHAR *p = strrchr(path, '\\');
	if (p != NULL) p++;
	else p = path;
	_tcscpy(p, VIET_FONT_FILE);

}

int CMainWnd::loadVietFont()
{
	TCHAR path[MAX_PATH];
	getFontFile(path, sizeof(path));
	if (AddFontResource(path) > 0) {
		SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		m_fontLoaded = 1;
		//fontInstalled = 1;
		return 1;
	}
	return 0;
}

void CMainWnd::cleanup()
{
	if (m_fontLoaded) {
		TCHAR path[MAX_PATH];
		getFontFile(path, sizeof(path));
		RemoveFontResource(path);
		m_fontLoaded = 0;
	}
}

//-----------------------------------------
HFONT CustomCreateFont(HDC hDC, LPCSTR face, int size)
{
	LOGFONT lf;
	if (face == NULL || face[0] == NULL)
		return (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	_tcscpy(lf.lfFaceName, face);
	lf.lfHeight = -MulDiv(size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	//lf.lfPitchAndFamily = 
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfWeight = FW_NORMAL;
	return CreateFontIndirect(&lf);
}


