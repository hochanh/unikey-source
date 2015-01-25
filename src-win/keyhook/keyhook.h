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
#ifndef _KEYHOOK_H
#define _KEYHOOK_H 1
#include <windows.h>

struct CodeInfo {
	DWORD DT[256];
	unsigned char BD[12][6];
	unsigned char BK[8];
	unsigned char BW[6];
	unsigned char BT[6];
	WORD ToUniH[256];
	WORD ToUniL[256];
	int multiBytes; // 2 bytes charset (Unicode, VNI, ....)
};

#define CTRL_SHIFT_SW 0
#define ALT_Z_SW 1

struct SharedMem {
	int Initialized;
	int vietKey;
	WORD keyMode;
	int inMethod;
	int freeMarking;
	int toneNextToVowel;
	int modernStyle;
	HHOOK keyHook,mouseHook;
	HWND hMainDlg;
	UINT iconMsgId;
//	UINT keyMsgId;
	HICON hVietIcon,hEnIcon;
	int iconShown;
	int switchKey;
	CodeInfo codeTable;
};

//----------------------------------------------------------------
// Interface functions 
LRESULT CALLBACK MyKeyHook(int code, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MyMouseHook(int code, WPARAM wParam, LPARAM lParam);
HINSTANCE GetVietHookDll();
void InitSharedMem(SharedMem *pPara);
void SwitchMode();
void SetKeyMode(WORD mode, int inMethod, CodeInfo *pTable);
void SetSwitchKey(int swKey);
void SetInputMethod(int method, DWORD *DT);
int IsVietnamese();
void ModifyStatusIcon();
void DeleteStatusIcon();
void SetKeyOptions(int freeMarking, int toneNextToVowel, int modernStyle);


#endif
