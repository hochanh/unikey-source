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
#include "util.h"

/*--------------------------------------------------------------------------
  Function: GenWindowProc

  Summary:  Standard WindowProc callback function that forwards Windows
            messages on to the CVirWindow::WindowProc method.  This
            Window procedure expects that it will receive a "this"
            pointer as the lpCreateParams member passed as part of the
            WM_NCCREATE message.  It saves the "this" pointer in the
            GWL_USERDATA field of the window structure.
----------------------------------------------------------------------------*/
LRESULT CALLBACK GenWindowProc(
						HWND hWnd,
						UINT uMsg,
						WPARAM wParam,
						LPARAM lParam)
{
  // Get a pointer to the window class object.
  CVirWindow* pWin = (CVirWindow*) GetWindowLong(hWnd, GWL_USERDATA);

  switch (uMsg)
  {
    case WM_NCCREATE:
      // Get the initial creation pointer to the window object
      pWin = (CVirWindow *) ((CREATESTRUCT *)lParam)->lpCreateParams;

      pWin->m_hWnd = hWnd;

      // Set its USERDATA DWORD to point to the window object
      SetWindowLong(hWnd, GWL_USERDATA, (long) pWin);
      break;
/*
    case WM_DESTROY:
      // This is our signal to destroy the window object.
      SetWindowLong(hWnd, GWL_USERDATA, 0);
	  if (pWin) {
		  delete pWin;
		  pWin = NULL;
	  }
      break;
*/
    default:
      break;
  }

  // Call its message proc method.
  if (NULL != pWin)
    return (pWin->WindowProc(uMsg, wParam, lParam));
  else
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}


/*--------------------------------------------------------------------------
  Method:   CVirWindow::Create

  Summary:  Envelopes the Windows' CreateWindow call.  Uses its
            window-creation data pointer to pass the 'this' pointer.
----------------------------------------------------------------------------*/
HWND CVirWindow::Create(
	   DWORD dwExStyle,
       LPTSTR lpszClassName,
       LPTSTR lpszWindowName,
       DWORD dwStyle,
       int x,
       int y,
       int nWidth,
       int nHeight,
       HWND hWndParent,
       HMENU hMenu,
       HINSTANCE hInst)
{
  // Remember the passed instance handle in a member variable of the
  // C++ Window object.
  m_hInst = hInst;

  m_hWnd = CreateWindowEx(dwExStyle,
				lpszClassName,
				lpszWindowName,
				dwStyle,
				x,
				y,
				nWidth,
				nHeight,
				hWndParent,
				hMenu,
				hInst,
				this);

  return (m_hWnd);
}

/*-------------------------------------------------------------------------
  Function: GenDialogProc

  Summary:  The general dialog procedure callback function.  Used by all
            CVirDialog class objects.  This procedure is the DialogProc.
            registered for all dialogs created with the CVirDialog class.
            It uses the parameter passed with the WM_INITDIALOG message
            to identify the dialog classes' "this" pointer which it then
            stores in the window structure's GWL_USERDATA field.
            All subsequent messages can then be forwarded on to the
            correct dialog class's DialogProc method by using the pointer
            stored in the GWL_USERDATA field.
-------------------------------------------------------------------------*/
BOOL CALLBACK GenDialogProc(
					HWND hWndDlg,
					UINT uMsg,
					WPARAM wParam,
					LPARAM lParam)
{
	// Get a pointer to the window class object.
	CVirDialog* pdlg = (CVirDialog*) GetWindowLong(hWndDlg, GWL_USERDATA);

	switch (uMsg) {
	case WM_INITDIALOG:
		// Get a pointer to the window class object.
		pdlg = (CVirDialog*) lParam;

		// Assign the m_hWnd member variable.
		pdlg->m_hWnd = hWndDlg;

		// Set the USERDATA word to point to the class object.
		SetWindowLong(hWndDlg, GWL_USERDATA, (long) pdlg);
		break;

	default:
		break;
	}

	// Call its message proc method.
	if (pdlg != (CVirDialog *) 0)
		return (pdlg->DialogProc(hWndDlg, uMsg, wParam, lParam));
	else
		return (FALSE);
}

/*-------------------------------------------------------------------------
  CVirDialog:constructor
-------------------------------------------------------------------------*/
CVirDialog::CVirDialog()
{
	m_hWnd = m_hWndOwner = NULL;
	m_hInst = NULL;
	m_lpszTemplate = NULL;
}


/*-------------------------------------------------------------------------
 Initialize the dialog
-------------------------------------------------------------------------*/
int CVirDialog::init(HINSTANCE hInst, LPCSTR lpszTemplate, HWND hWndOwner)
{
	m_hInst = hInst;
	m_lpszTemplate = lpszTemplate;
	m_hWndOwner = hWndOwner;
	return 1;
}

/*-------------------------------------------------------------------------
  Method:   CVirDialog::ShowDialog

  Summary:  like DialogBox Windows API function.
-------------------------------------------------------------------------*/
int CVirDialog::ShowDialog()
{
  int iResult;

  // Create and show the dialog on screen. Pass the 'this' pointer to the
  // dialog object so that it can be assigned inside the dialog object
  // during WM_INITDIALOG and later available to the dailog procedure
  // via the GWL_USERDATA associated with the dialog window.
  iResult = ::DialogBoxParam(
                m_hInst,
                m_lpszTemplate,
                m_hWndOwner,
                (DLGPROC)::GenDialogProc,
                (long)this);

  return (iResult);
}

//------------------------------------------
BOOL CVirDialog::DialogProc(
						HWND hWndDlg,
						UINT uMsg,
						WPARAM wParam,
						LPARAM lParam)
{
	switch (uMsg) {
	case WM_COMMAND:
		return onCommand(wParam, lParam);
	case WM_INITDIALOG:
		return onInitDialog();
	}
	return FALSE;
}

//------------------------------------------
BOOL CVirDialog::onCommand(WPARAM wParam, LPARAM lParam)
{
	switch (wParam) {
	case IDOK:
		if (onOK())
			EndDialog(m_hWnd, IDOK);
		return TRUE;
	case IDCANCEL:
		if (onCancel())
			EndDialog(m_hWnd, IDCANCEL);
		return TRUE;
	}
	return FALSE;
}

//------------------------------------------
BOOL CVirDialog::onOK()
{
	return TRUE;
}


//------------------------------------------
BOOL CVirDialog::onCancel()
{
	return TRUE;
}


//------------------------------------------
BOOL CVirDialog::onInitDialog()
{
	return TRUE;
}
