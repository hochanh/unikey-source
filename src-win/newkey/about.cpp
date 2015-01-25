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
#include "resource.h"
#include "mainwnd.h"
#include "about.h"
#include "userpref.h"

#define UNIKEY_HOME_PAGE _TEXT("http://unikey.hypermart.net")
#define AUTHOR_EMAIL _TEXT("mailto:longp@cslab.felk.cvut.cz")

extern HINSTANCE AppInstance;
extern CUserPref UserPref; // user preference

DlgItemInfo VnAboutDlgItems[] = {
	{IDC_AUTHOR,		_T("T¸c gi¶: Ph¹m Kim Long, §¹i häc Kü thuËt Praha")},
	{IDC_COPYRIGHT,		_T("Copyright (C) 1998 - 2001 Ph¹m Kim Long")},
	{IDC_INFO,			_T(" UniKey lµ ch­¬ng tr×nh m· nguån më. B¹n cã thÓ ph©n phèi,\n"
						   " söa ®æi UniKey tu©n theo The GNU General Public License.\n"
						   " UniKey hç trî c¸c hÖ ®iÒu hµnh: Windows 9x/ME/NT/2000.")}};

BOOL CAboutDlg::DialogProc(
	HWND hDlg,  // handle to dialog box
	UINT uMsg,     // message
	WPARAM wParam, // first message parameter
	LPARAM lParam  // second message parameter
)
{
	switch (uMsg) {
	case WM_DRAWITEM:
		// Send the reflection message back to the control
		return SendDlgItemMessage(hDlg, wParam, WM_DRAWITEM, wParam, lParam);
	case WM_CTLCOLORSTATIC:
		// Send the reflection message back to the control
		return SendMessage((HWND)lParam, WM_CTLCOLORSTATIC, wParam, 0);
	case WM_DESTROY:
		DestroyCursor(m_hLinkCursor);
		if (m_hStdFont != NULL) {
			DeleteObject(m_hStdFont);
			m_hStdFont = NULL;
		}
		break;
	}
	return CVirDialog::DialogProc(hDlg, uMsg, wParam, lParam);
}

//---------------------------------------------
BOOL CAboutDlg::onInitDialog()
{
	if (!CVirDialog::onInitDialog()) return FALSE;
	if (m_langID == 1) {
		setItemsText(1);
		setDlgFont(1);
	}

	//setup buttons
	m_okBtn.init(m_hInst, m_hWnd, GetDlgItem(m_hWnd, IDOK));
	//m_okBtn.setTextColor(RGB(255, 0, 0));
	m_okBtn.setIcon(IDI_OK, 16, 16);
	m_okBtn.setDefaultButton(TRUE);
	//setup hyper-link labels
	m_hLinkCursor = (HCURSOR)LoadImage(
								m_hInst, 
								MAKEINTRESOURCE(IDC_HAND_CURSOR), 
								IMAGE_CURSOR, 32, 32, 0);
	CLinkLabel::setLinkCursor(m_hLinkCursor);
	m_webLink.init(m_hInst, m_hWnd, GetDlgItem(m_hWnd, IDC_WEB_LINK), UNIKEY_HOME_PAGE);
	m_mailLink.init(m_hInst, m_hWnd, GetDlgItem(m_hWnd, IDC_MAIL_LINK), AUTHOR_EMAIL);

	return TRUE;
}
//----------------------------------------------
// set text labels according to the chosen GUI language
// langID = 0 -> English (default)
//----------------------------------------------
void CAboutDlg::setItemsText(int langID)
{
	int items, i;
	DlgItemInfo *texts;
	if (langID == 0)
		return; // default language is English, the texts are taken from the resource
	texts = VnAboutDlgItems;
	items = sizeof(VnAboutDlgItems)/sizeof(DlgItemInfo);
	for (i=0; i<items; i++)
		SendDlgItemMessage(m_hWnd, texts[i].id, WM_SETTEXT, 0, (LPARAM)texts[i].text);
}

//----------------------------------------------
// set font for dialog items accoring to the selected GUI language
// langID = 0 -> English (default)
//----------------------------------------------
void CAboutDlg::setDlgFont(int langID)
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

//------------------------------------------------
void DoAbout(HINSTANCE hInst, HWND hParent, int langID)
{
	CAboutDlg dlg(langID);
	dlg.init(hInst, MAKEINTRESOURCE(IDD_ABOUT), hParent);
	dlg.ShowDialog();
}

