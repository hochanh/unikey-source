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
#ifndef _VIETKEY_H
#define _VIETKEY_H 1

#define KEY_BUFSIZE 40
#define KEYS_MAINTAIN 20

class VietKey {
private:
	int keys;
	unsigned char buf[KEY_BUFSIZE];
	int lowerCase[KEY_BUFSIZE];
	int lastWConverted; // used in TELEX, WConverted =1 when last single w was converted to u+
	DWORD  *DT;
	unsigned char (*BD)[6];
	unsigned char *BK;
	unsigned char *BW;
	unsigned char *BT;
	WORD *ToUniH;
	WORD *ToUniL;

	int keyCategory(unsigned char c);
	void putBreveMark(unsigned char c);
	void doubleChar(unsigned char c);
	void putToneMark(unsigned char c);
	void shortKey(unsigned char c);
	void vniDoubleCharMark(unsigned char c);
//	int	 getToneIndex(unsigned char c);
	void throwBuf();  // used when buffer is full
public:
	VietKey();
	int keysPushed,backs;
	unsigned char ansiPush[(KEY_BUFSIZE+2)*3]; // 1 vienameses character may requires 3 key strokes (as in the case of VIQR)
	WORD uniPush[KEY_BUFSIZE+2];
	void clearBuf();
	void process(unsigned char ch);
	void setCodeTable(CodeInfo * pInfo);
};

#endif
