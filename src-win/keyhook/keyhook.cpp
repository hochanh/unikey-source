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
#include <memory.h>
#include "keycons.h"
#include "keyhook.h"
#include "vietkey.h"

const long IsExtended = 0x1000000,
		   FlagScancode = 0xFF0000,
           IsALT = 0x20000000,
		   IsPrevDown = 0x40000000,
		   IsReleased = 0x80000000,
		   KeyUp = 0xC0000000;

VietKey VnKbd;

SharedMem * pShMem = NULL ;
HINSTANCE MyInstance = NULL;

unsigned char  KeyState[256];
unsigned short CharBuf;
int BackTracks;
int ClipboardIsEmpty = 1;
int PendingSwitch = 0;
//DWORD TlsIdx;

void InitProcess();
void ClearClipboard();
void ResetBuffer();

//////////////////////////////////
/*
struct ThreadData {
	HHOOK GetMsgHook;
	int receivingClipboard;
};
*/
//void InitThread();


int CheckSwitchKey(WPARAM wParam, LPARAM lParam);
int CheckBack(WPARAM wParam);
void PushBacks();
void PushBuffer(HWND wndFocused);


#define ID_VIETKEY_ICON 1000


UINT VK_SHIFT_SCAN;
UINT VK_BACK_SCAN;
UINT VK_INSERT_SCAN;
UINT VK_PAUSE_SCAN;

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD reason, LPVOID reserved)
{
    HANDLE hMapObject = NULL;   /* handle to file mapping */ 
    BOOL fInit, fIgnore; 
//	ThreadData *pThread;

    switch (reason) { 
 		case DLL_PROCESS_ATTACH: 
 
            /* Create a named file mapping object */ 
			MyInstance =  hModule;
            hMapObject = CreateFileMapping( 
                (HANDLE) 0xFFFFFFFF, /* use paging file    */ 
                NULL,                /* no security attr.  */ 
                PAGE_READWRITE,      /* read/write access  */ 
                0,                   /* size: high 32-bits */ 
                sizeof(SharedMem),   /* size: low 32-bits  */ 
                _TEXT("UniKeyHookDllFileMap"));/* name of map object */ 
            if (hMapObject == NULL) 
                return FALSE; 
			
			fInit = (GetLastError() != ERROR_ALREADY_EXISTS);
            /* Get a pointer to the file-mapped shared memory. */ 
 
            pShMem = (SharedMem *) MapViewOfFile( 
				hMapObject,     /* object to map view of    */ 
                FILE_MAP_WRITE, /* read/write access        */ 
                0,              /* high offset:   map from  */ 
                0,              /* low offset:    beginning */ 
                0);             /* default: map entire file */ 
            if (pShMem == NULL) 
                return FALSE; 
			if (fInit) 
				pShMem->Initialized = 0; 
			else
				InitProcess();
//			TlsIdx = TlsAlloc();
//			InitThread();
         break; 
 
        /* 
         * The DLL is detaching from a process due to 
         * process termination or a call to FreeLibrary. 
         */ 
/*
		  case DLL_THREAD_ATTACH:
			  InitThread();
			  break;
		  case DLL_THREAD_DETACH:
			  pThread = (ThreadData *)TlsGetValue(TlsIdx);
			  if (pThread != NULL) 
				delete pThread;
			  break;
*/
		case DLL_PROCESS_DETACH: 
 
            /* Unmap shared memory from the process's address space. */ 
 
            fIgnore = UnmapViewOfFile(pShMem); 
 
            /* Close the process's handle to the file-mapping object. */ 
 
            fIgnore = CloseHandle(hMapObject); 
//			TlsFree(TlsIdx);
            break; 
 
        default: 
          break; 
 
  } 
 
  return TRUE; 
  UNREFERENCED_PARAMETER(hModule); 
  UNREFERENCED_PARAMETER(reserved); 
 
}

/*
void InitThread()
{
	ThreadData *pThread;
	pThread = new ThreadData();
	memset(pThread, 0, sizeof(ThreadData));
	TlsSetValue(TlsIdx, pThread);
}
*/
void InitProcess()
{
	BackTracks = 0;
	VnKbd.setCodeTable(&pShMem->codeTable);
	VK_SHIFT_SCAN = MapVirtualKey(VK_SHIFT,0);
	VK_INSERT_SCAN = MapVirtualKey(VK_INSERT, 0);
	VK_BACK_SCAN = MapVirtualKey(VK_BACK, 0);
	VK_PAUSE_SCAN = MapVirtualKey(VK_PAUSE, 0);
}

void InitSharedMem(SharedMem *pPara) 
{
	*pShMem = *pPara;
	InitProcess();
	pShMem->Initialized = 1;
}
	

LRESULT CALLBACK MyKeyHook(int code, WPARAM wParam, LPARAM lParam)
{
	int keyType;
	if (!pShMem->Initialized) return 0;
	if (code < 0) 
		return CallNextHookEx(pShMem->keyHook,code,wParam,lParam);
	if (CheckSwitchKey(wParam,lParam)) 
		return 1;
	if (pShMem->vietKey && code == HC_ACTION && !ClipboardIsEmpty) {
		if (wParam != VK_INSERT && wParam != VK_SHIFT && !(lParam & IsReleased)) {
			// postpone this key
			keybd_event(wParam, 
						HIBYTE(LOWORD(lParam)), 
						(lParam & IsExtended)? KEYEVENTF_EXTENDEDKEY : 0,
						0);
			return 1;
		}
		if (wParam == VK_INSERT && (lParam & IsReleased)) {
			ClearClipboard();
		}
	}
		
	if (pShMem->vietKey && (code == HC_ACTION) && !(lParam & IsReleased)) {
		if (CheckBack(wParam))	return 1;
		if (BackTracks > 0) {
			if (wParam != VK_BACK) {
				keybd_event(wParam, 
						HIBYTE(LOWORD(lParam)), 
						(lParam & IsExtended)? KEYEVENTF_EXTENDEDKEY : 0,
						0);
				return 1;
			}
			return CallNextHookEx(pShMem->keyHook,code,wParam,lParam);
		}
		GetKeyboardState(KeyState);
		if ((KeyState[VK_CONTROL] & 0x80) == 0 && (KeyState[VK_MENU] & 0x80) == 0) {
			keyType = ToAscii(wParam,UINT(HIWORD(lParam)),KeyState,&CharBuf,0);
			if (keyType == 1) {
				unsigned char c = (unsigned char)CharBuf;
				VnKbd.process(c);
				if (VnKbd.backs!=0 || VnKbd.keysPushed!=0) {
					PushBacks();
					return 1;
				}
				//SendMessage(GetFocus(), WM_CHAR, CharBuf, 1);
				//return 1;
			}
			else if (wParam!=VK_SHIFT && wParam!=VK_INSERT)
				ResetBuffer();
		}
		else ResetBuffer();
	}
	return CallNextHookEx(pShMem->keyHook,code,wParam,lParam);
}


void PushBuffer(HWND wndFocused)
{
	PostMessage(wndFocused,WM_KEYUP,VK_PAUSE,
		(LPARAM(VK_PAUSE_SCAN)<<16)+LPARAM(IsReleased)+1);
	if (pShMem->keyMode == UNICODE_CHARSET) {
/*
		for (int i=0; i<VnKbd.keysPushed; i++)
			PostMessage(wndFocused,WM_IME_CHAR, VnKbd.uniPush[i],1 | IsExtended);
*/
		OpenClipboard(wndFocused);  //OpenClipboard(NULL);
		EmptyClipboard();

		VnKbd.uniPush[VnKbd.keysPushed] = 0; // null-terminated
		VnKbd.ansiPush[VnKbd.keysPushed] = 0;
		VnKbd.keysPushed++;

		HGLOBAL hBuf = GlobalAlloc(GMEM_MOVEABLE, sizeof(WORD) * VnKbd.keysPushed);
		HGLOBAL hBufAnsi = GlobalAlloc(GMEM_MOVEABLE, VnKbd.keysPushed);

		LPVOID pBuf = GlobalLock(hBuf);
		LPVOID pBufAnsi = GlobalLock(hBufAnsi);
		memcpy(pBuf, VnKbd.uniPush, sizeof(WORD) * VnKbd.keysPushed);
		memcpy(pBufAnsi, VnKbd.ansiPush, VnKbd.keysPushed);
		GlobalUnlock(hBuf);
		GlobalUnlock(hBufAnsi);

		SetClipboardData(CF_UNICODETEXT, hBuf);
		SetClipboardData(CF_TEXT, hBufAnsi);

		CloseClipboard();

		GetKeyboardState(KeyState);
		keybd_event(VK_SHIFT, 0, 0, 0);
		keybd_event(VK_INSERT, 0, KEYEVENTF_EXTENDEDKEY, 0);
		keybd_event(VK_INSERT, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

		if ((KeyState[VK_SHIFT] & 0x80) == 0 || (KeyState[VK_RSHIFT] & 0x80) != 0)
			keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
		ClipboardIsEmpty = 0;
	}
	else {
		for (int i=0; i<VnKbd.keysPushed; i++)
			PostMessage(wndFocused,WM_CHAR, VnKbd.ansiPush[i],1);
	}
	return;
}

void PushBacks()
{
	if (VnKbd.backs == 0) {
		BackTracks = 0;
		PushBuffer(GetFocus());
	}
	else {
		BackTracks = VnKbd.backs+1;
		keybd_event(VK_BACK, VK_BACK_SCAN,0,0);
	}
	return;
}

int CheckBack(WPARAM wParam)
{
	if (wParam==VK_BACK && BackTracks>0) {
		BackTracks --;
		if (BackTracks == 0) {
			PushBuffer(GetFocus());
			return 1;
		}
		else keybd_event(VK_BACK, VK_BACK_SCAN, 0, 0);
	}
	return 0;
}

int CheckSwitchKey(WPARAM wParam, LPARAM lParam)
{
	if (!ClipboardIsEmpty)
		return 0;
	UINT key1, key2;
	switch (pShMem->switchKey) {
	case CTRL_SHIFT_SW:
		key1 = VK_SHIFT;
		key2 = VK_CONTROL;
		break;
	default:
		key1 = VK_MENU;
		key2 = 'Z';
		break;
	}

	if (wParam == key1 || wParam == key2) {
		if (lParam & IsReleased) {
			if (PendingSwitch) {
				PendingSwitch = 0;
				SwitchMode();
				MessageBeep(0xFFFFFFFF);
				return 1;
			}
		}
		else {
			if ((GetKeyState(key1) & 0x8000) && (GetKeyState(key2) & 0x8000))
				PendingSwitch = 1;
		}
	}
	else PendingSwitch = 0;
	return 0;
}

LRESULT CALLBACK MyMouseHook(int code, WPARAM wParam, LPARAM lParam)
{
	if (pShMem->Initialized && code == HC_ACTION && wParam == WM_LBUTTONDOWN)
			ResetBuffer();
	return CallNextHookEx(pShMem->mouseHook,code,wParam,lParam);
}

void SwitchMode()
{
	if (pShMem->vietKey) pShMem->vietKey = 0;
	else pShMem->vietKey = 1;
	if (pShMem->iconShown) ModifyStatusIcon();
	ClearClipboard();
}


void ModifyStatusIcon()
{
    NOTIFYICONDATA tnid; 

    tnid.cbSize = sizeof(NOTIFYICONDATA);
    tnid.hWnd = pShMem->hMainDlg;
	tnid.uID = ID_VIETKEY_ICON;
	if (pShMem->vietKey) {
		tnid.hIcon = pShMem->hVietIcon;
		lstrcpy(tnid.szTip,_TEXT("Click to turn off Vienamese mode"));

	} else {
		tnid.hIcon = pShMem->hEnIcon;
		lstrcpy(tnid.szTip,_TEXT("Click to turn on Vienamese mode"));
	}
    tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
    tnid.uCallbackMessage = pShMem->iconMsgId;
	Shell_NotifyIcon(pShMem->iconShown?NIM_MODIFY:NIM_ADD, &tnid);
	pShMem->iconShown = 1;
} 

void DeleteStatusIcon()
{
    NOTIFYICONDATA tnid; 
	if (pShMem->iconShown) {
		tnid.cbSize = sizeof(NOTIFYICONDATA); 
		tnid.hWnd = pShMem->hMainDlg;
		tnid.uID = ID_VIETKEY_ICON;
		Shell_NotifyIcon(NIM_DELETE, &tnid); 
		pShMem->iconShown = 0;
	}
}

int IsVietnamese()
{
	return pShMem->vietKey;
}

void SetKeyMode(WORD mode, int inMethod, CodeInfo *pTable)
{
	pShMem->keyMode = mode;
	pShMem->inMethod = inMethod;
	pShMem->codeTable = *pTable;
	ResetBuffer();
}


void ClearClipboard()
{
	ClipboardIsEmpty = 1;
//	OpenClipboard(NULL);
//	EmptyClipboard();
//	CloseClipboard();
}


void ResetBuffer()
{
	VnKbd.clearBuf();
	ClearClipboard();
}

void SetSwitchKey(int swKey)
{
	pShMem->switchKey = swKey;
}

void SetInputMethod(int method, DWORD *DT)
{
	pShMem->inMethod = method;
	memcpy(pShMem->codeTable.DT, DT, sizeof(pShMem->codeTable.DT));
	ResetBuffer();
}


HINSTANCE GetVietHookDll()
{
	return MyInstance;
}


void SetKeyOptions(int freeMarking, int toneNextToVowel, int modernStyle)
{
	pShMem->freeMarking = freeMarking;
	pShMem->toneNextToVowel = toneNextToVowel;
	pShMem->modernStyle = modernStyle;
}
