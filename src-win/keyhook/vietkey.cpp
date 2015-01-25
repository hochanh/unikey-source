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
#include <ctype.h>
#include "keycons.h"
#include "keyhook.h"
#include "vietkey.h"

// key category constants
#define BACK_CHAR 8
#define BREVE_MARK 1	// for marks: ( in "a(", and also "+" in u+, o+
#define TONE_MARK 2		// tone marks, eg. a', a`,...
#define DOUBLE_KEY 3
#define SHORT_KEY 4
#define VOWEL_CHAR 5
#define SEPARATOR_KEY 6
#define VNI_DOUBLE_CHAR_MARK 7 // in VNI method

#define VNI_CIRCUMFLEX_INDEX 1
#define VNI_HORN_INDEX 2
#define VNI_BREVE_INDEX 3
#define VNI_D_INDEX 4

#define MAX_AFTER_VOWEL 2  // the max number of characters after a vowels
#define MAX_VOWEL_SQUENCE 3
#define MAX_MODIFY_LENGTH 6

extern SharedMem *pShMem;

// for processing in VIQR mode
unsigned char VIQRsym[] = {'\'','`','?','~','.','^','(','+'};
//unsigned char ShortcutReverse[] = {'o', 'u', 'u', 'U'};  // We have 4 shortcuts: [,],w,W (see tcvn.cpp)
/////
unsigned char DoubleReverse[] = {'d','D','e','E','a','A','o','O'};
unsigned char WReverse[] = {'a','A','o','O','u','U'};

VietKey::VietKey() {
	keys = 0;
	DT = 0;
	BD = 0;
	BK = 0;
	BW = 0;
	BT = 0;
	lastWConverted = 0;
}

unsigned char ChangedChar, OldChar;

void VietKey::process(unsigned char c)
{
	int i, j, kieu, k;
	int thisWConverted;
	static unsigned char tempPush[KEY_BUFSIZE*3];
	WORD mapping, map2;
	keysPushed = 0;
	backs = 0;
	unsigned char t;
	OldChar = c;
	thisWConverted = 0;

	if (pShMem->codeTable.multiBytes)
		c = tolower(c);
	//check VIQR symbol
	/* // DO NOT DELETE THIS PART
	if (pShMem->keyMode == VIQR_CHARSET) {
		for (i=0; c!=VIQRsym[i] && i<sizeof(VIQRsym); i++);
		if (i<sizeof(VIQRsym)) { // found VIQR symbol
			if (i<5) {
				kieu = TONE_MARK;
				c = i;
			}
			else if (i==5) {
				kieu = DOUBLE_KEY;
			}
			else kieu = BREVE_MARK;
		}
		else if (c == 'd') 
			kieu = DOUBLE_KEY;
		else 
			kieu = keyCategory(c);
	}
	else kieu = keyCategory(c);
	*/
	kieu = keyCategory(c);

	switch(kieu){
	case BREVE_MARK:
		if (pShMem->inMethod == TELEX_INPUT && lastWConverted && (c == 'w' || c == 'W'))
			shortKey(c);
		else {
			putBreveMark(c);
			if (pShMem->inMethod == TELEX_INPUT && keysPushed == 0 && backs == 0 && (c == 'w' || c == 'W')) {
				shortKey(c); // Special case for W key in TELEX mode.
				thisWConverted = 1;
			}
		}
		break;
	case DOUBLE_KEY:
		doubleChar(c);
		break;
	case TONE_MARK:
		putToneMark(c);
		break;
	case SHORT_KEY:
		shortKey(c);
		break;
	case VNI_DOUBLE_CHAR_MARK:
		vniDoubleCharMark(c);
		break;
	case SEPARATOR_KEY:
		if (c==BACK_CHAR && keys>0) {
			backs = 1;
			if (pShMem->keyMode == VIQR_CHARSET) {
				mapping = lowerCase[keys-1]? ToUniL[buf[keys-1]] : ToUniH[buf[keys-1]];
				if (HIBYTE(mapping))
					backs++;
				t = LOBYTE(mapping);
				mapping = lowerCase[keys-1]? ToUniL[t] : ToUniH[t];
				if (HIBYTE(mapping))
					backs++;
			}
			else if (pShMem->keyMode != UNICODE_CHARSET && pShMem->codeTable.multiBytes) {
				mapping = (lowerCase[keys-1])? ToUniL[buf[keys-1]] : ToUniH[buf[keys-1]];
				if (HIBYTE(mapping))
					backs++;
			}
			keys--;
		}
		else 
			clearBuf();
		lastWConverted = 0;
		return;
	}
	lastWConverted = thisWConverted;
	if (keysPushed==0 && backs==0) {
		//t = toupper(c);
		if (keys==KEY_BUFSIZE) 
			throwBuf();
		lowerCase[keys] = islower(OldChar);
		buf[keys] = c;
		keys++;
	}
	if (keysPushed == 0)
		return;
	if (keys < keysPushed && pShMem->codeTable.multiBytes) {
		keysPushed = 0;
		backs = 0;
		return;
	}
	if (pShMem->keyMode == UNICODE_CHARSET) {
		for (i = 0, j = keys-keysPushed; i < keysPushed; i++, j++) {
			uniPush[i] = (lowerCase[j]) ? ToUniL[ansiPush[i]] : ToUniH[ansiPush[i]];
		}
		return;
	}

	if (pShMem->keyMode == VIQR_CHARSET) {
		memcpy(tempPush, ansiPush, keysPushed);
		for (i = k = 0, j=keys-keysPushed; i<keysPushed; i++, j++) {
			mapping = lowerCase[j]? ToUniL[tempPush[i]] : ToUniH[tempPush[i]];
			t = LOBYTE(mapping);
			map2 = lowerCase[j]? ToUniL[t] : ToUniH[t];
			ansiPush[k++] = LOBYTE(map2);
			if (HIBYTE(map2)) {
				ansiPush[k++] = HIBYTE(map2);
				if (backs > 0 && i > 0)
					backs++;
			}
			if (HIBYTE(mapping)) {
				ansiPush[k++] = HIBYTE(mapping);
				if (backs > 0 && i > 0) // if backs was already = 0, we never have to push any more back
					backs++;
			}
		}
		// determine the number of backs
		if (backs > 0) {
			// check the changed char
			mapping = lowerCase[keys-keysPushed]? ToUniL[ChangedChar] : ToUniH[ChangedChar];
			if (HIBYTE(mapping))
				backs++;
			t = LOBYTE(mapping);
			mapping = lowerCase[keys-keysPushed]? ToUniL[t] : ToUniH[t];
			if (HIBYTE(mapping))
				backs++;
		}
		keysPushed = k;
		return;
	}

	if (pShMem->codeTable.multiBytes) {
		memcpy(tempPush, ansiPush, keysPushed);
		for (i = k = 0, j=keys-keysPushed; i<keysPushed; i++, j++) {
			mapping = lowerCase[j]? ToUniL[tempPush[i]] : ToUniH[tempPush[i]];
			ansiPush[k++] = LOBYTE(mapping);
			if (HIBYTE(mapping)) {
				ansiPush[k++] = HIBYTE(mapping);
				if (backs > 0 && i > 0) // if backs was already = 0, we never have to push any more back
					backs++;
			}
		}
		// determine the number of backs
		if (backs > 0) {
			mapping = lowerCase[keys-keysPushed]? ToUniL[ChangedChar] : ToUniH[ChangedChar];
			if (HIBYTE(mapping))
				backs++;
		}
		keysPushed = k;
		return;
	}
}

void VietKey::clearBuf()
{
	keys = 0;
	lastWConverted = 0;
}


int VietKey::keyCategory(unsigned char c)
{
	long index, attr;
	attr = DT[c];
/*
	int macroIdx;
	// check for macro first
	if ((macroIdx = (DT[c] >> 4) & 0x7) > 0) {
		if ((DT[c] >> 12) & 1) {
			if (keys > 0) {
				index = (DT[buf[keys-1]] >> 7) & 0xF;
				if (index > 0 && buf[keys-1] != BT[macroIdx-1])
					return BREVE_MARK;
			}
		}
		return SHORT_KEY;
	}
*/
	if (ATTR_IS_BREVE(attr) > 0)
		return BREVE_MARK;

	if (ATTR_TONE_INDEX(attr) > 0)
		return TONE_MARK;

	index = ATTR_DBCHAR_INDEX(attr);
	if (pShMem->inMethod != VNI_INPUT && index > 0 && index < 9)
		return DOUBLE_KEY;
	if (pShMem->inMethod == VNI_INPUT && ATTR_VNI_DOUBLE_INDEX(attr) > 0)
		return VNI_DOUBLE_CHAR_MARK;

	if (ATTR_MACRO_INDEX(attr) > 0)
		return SHORT_KEY;

	if (ATTR_IS_SEPARATOR(attr) > 0)
		return SEPARATOR_KEY;
	return 0;
}

void VietKey::putBreveMark(unsigned char c)
{
	int i, k;
	long attr;
	unsigned char t, newChar;
	int leftMost;

	int index, index_c, toneIndex = 0;
	// determine the position to drop the breve mark
	i = keys-1;
	if (pShMem->inMethod == VNI_INPUT)
		index_c = ATTR_VNI_DOUBLE_INDEX(DT[c]);
	leftMost = pShMem->freeMarking ? 0 : keys-1;
	leftMost = max(leftMost, keys - MAX_MODIFY_LENGTH);

	while (i >= leftMost) {
		attr = DT[buf[i]];
		toneIndex = ATTR_CURRENT_TONE(DT[buf[i]]);
		if (toneIndex == 0 || toneIndex == 6)
			index = ATTR_DBCHAR_INDEX(attr);
		else
			index = ATTR_DBCHAR_INDEX(DT[BD[ATTR_VOWEL_INDEX(attr)-1][5]]);
		if (index > 4) { // from 11 are the indexes of VNI double-charater marks
			if (pShMem->inMethod == VNI_INPUT) {
				if ((index_c == VNI_HORN_INDEX && index > 6) ||
					(index_c == VNI_BREVE_INDEX && index <= 6))
					break;
			}
			else 
				break;
		}
		else if (ATTR_IS_SEPARATOR(attr)  || ATTR_IS_SOFT_SEPARATOR(attr))
			break;
		i--;
	}
	if (i < leftMost || (index <= 4))
		return;

	if (pShMem->freeMarking && i > 0) {
		unsigned char prevChar;
		int tmpIdx;
		if (ATTR_VOWEL_INDEX(DT[prevChar = buf[i-1]]) > 0)
			prevChar = BD[ATTR_VOWEL_INDEX(DT[prevChar])-1][5];

		tmpIdx = ATTR_DBCHAR_INDEX(DT[prevChar]);
		if (tmpIdx > 4)
			prevChar = WReverse[tmpIdx-4-1];
		if ((prevChar == 'o' || prevChar == 'O' || prevChar == 'u' || prevChar == 'U') && 
			(buf[i] == 'u' || buf[i] == 'U')) {
			i--;
			toneIndex = ATTR_CURRENT_TONE(DT[buf[i]]);
		}

		if (i > 0) {
			if (ATTR_VOWEL_INDEX(DT[prevChar = buf[i-1]]) > 0)
				prevChar = BD[ATTR_VOWEL_INDEX(DT[prevChar])-1][5];
			if ((prevChar == 'U' || prevChar == 'u') &&
				(i == 1 || (i > 1 && buf[i-2] != 'q' && buf[i-2] != 'Q'))) {
				t = buf[i];
				if (ATTR_VOWEL_INDEX(DT[t]) > 0)
					t = BD[ATTR_VOWEL_INDEX(DT[t])-1][5];
				tmpIdx = ATTR_DBCHAR_INDEX(DT[t]) - 4;
				t = toupper(t);
				if ((t == 'A' && pShMem->inMethod != VNI_INPUT) ||	
					((t == 'O' || WReverse[tmpIdx-1] == 'o' || WReverse[tmpIdx-1] == 'O') && i != keys-1)) {
					i--;
					toneIndex = ATTR_CURRENT_TONE(DT[buf[i]]);
				}
			}
		}
	}

	if (toneIndex == 0 || toneIndex == 6) {
		index = ATTR_DBCHAR_INDEX(DT[buf[i]]) - 4;
		newChar = BW[index - 1];
	}
	else {
		index = ATTR_DBCHAR_INDEX(DT[BD[ATTR_VOWEL_INDEX(DT[buf[i]])-1][5]]) - 4;
		newChar = BD[ATTR_VOWEL_INDEX(DT[BW[index-1]])-1][toneIndex-1];
	}
	if (newChar != buf[i]) {
		if (pShMem->keyMode == VIQR_CHARSET) {
			if ((buf[i]=='a' && c==VIQRsym[7]) || (buf[i] != 'a' && c == VIQRsym[6]))
				return;
		}
		backs = keys - i;
		ChangedChar = buf[i];
		ansiPush[keysPushed++] = buf[i] = newChar;
		for (k = i+1; k < keys; k++)
			ansiPush[keysPushed++] = buf[k];
	}
	else {
		// duplicate, convert back to english
		backs = keys - i;
		ChangedChar = buf[i];
		if (toneIndex == 0 || toneIndex == 6)
			ansiPush[keysPushed++] = buf[i] = WReverse[index-1];
		else
			ansiPush[keysPushed++] = buf[i] = BD[ATTR_VOWEL_INDEX(DT[WReverse[index-1]])-1][toneIndex-1];
		for (k = i+1; k < keys; k++)
			ansiPush[keysPushed++] = buf[k];
		if (keys == KEY_BUFSIZE)
			throwBuf();
		lowerCase[keys] = islower(OldChar);
		buf[keys] = ansiPush[keysPushed++] = c;
		keys++;
	}
}

void VietKey::doubleChar(unsigned char c)
{
	int i, k, leftMost;
	long attr;
	unsigned char newChar;

	int index, index_c, toneIndex = 0;
	// determine the position to drop the cirumflex mark
	i = keys-1;
	if (pShMem->inMethod == VNI_INPUT)
		index_c = ATTR_VNI_DOUBLE_INDEX(DT[c]);
	else
		index_c = ATTR_DBCHAR_INDEX(DT[c]);
	leftMost = pShMem->freeMarking ? 0 : keys -1;
	leftMost = max(leftMost, keys - MAX_MODIFY_LENGTH);

	while (i >= leftMost) {
		attr = DT[buf[i]];
		toneIndex = ATTR_CURRENT_TONE(DT[buf[i]]);
		if (toneIndex == 0 || toneIndex == 6)
			index = ATTR_DBCHAR_INDEX(attr);
		else
			index = ATTR_DBCHAR_INDEX(DT[BD[ATTR_VOWEL_INDEX(attr)-1][5]]);
		if (index > 0 && index < 9) {
			if (pShMem->keyMode == VIQR_CHARSET && c == VIQRsym[5])
				break;
			if (pShMem->inMethod == VNI_INPUT) {
				if ( (index_c == VNI_CIRCUMFLEX_INDEX && index > 2) ||
					 (index_c == VNI_D_INDEX && index <= 2) )
					break;
			}
			else if (index  == index_c)
				break;
		}
		else if (ATTR_IS_SEPARATOR(attr)  || ATTR_IS_SOFT_SEPARATOR(attr)) {
			break;
		}
		i--;
	}
	if (i < leftMost || index == 0 || index >= 9)
		return;

	if (pShMem->keyMode == VIQR_CHARSET && c==VIQRsym[5] && (buf[i] == 'd' || buf[i] == 'D'))
		return;

	if (toneIndex == 0 || toneIndex == 6)
		newChar = BK[index-1];
	else
		newChar = BD[ATTR_VOWEL_INDEX(DT[BK[index-1]])-1][toneIndex-1];

	if (newChar != buf[i]) {
		backs = keys - i;
		ChangedChar = buf[i];
		ansiPush[keysPushed++] = buf[i] = newChar;
		for (k = i+1; k < keys; k++)
			ansiPush[keysPushed++] = buf[k];
	}
	else {
		backs = keys - i;
		ChangedChar = buf[i];
		if (toneIndex == 0 || toneIndex == 6)
			ansiPush[keysPushed++] = buf[i] = DoubleReverse[index-1];
		else
			ansiPush[keysPushed++] = buf[i] = BD[ATTR_VOWEL_INDEX(DT[DoubleReverse[index-1]])-1][toneIndex-1];
		for (k = i+1; k < keys; k++)
			ansiPush[keysPushed++] = buf[k];
		if (keys == KEY_BUFSIZE)
			throwBuf();
		lowerCase[keys] = islower(OldChar);
		buf[keys] = ansiPush[keysPushed++] = c;
		keys++;
	}
}


void VietKey::shortKey(unsigned char c)
{
	unsigned char newChar;
	int duplicate;
	int index = ATTR_MACRO_INDEX(DT[c]);
	newChar = BT[index-1];
	keysPushed = 0;

	duplicate = (keys > 0) && (buf[keys-1] == newChar);
	if (duplicate) {
		// convert back to english
		ChangedChar = buf[keys-1];
		buf[keys-1] = c;
		ansiPush[keysPushed++] = c;
		backs = 1;
		return;
	}

	backs = 0;
	ansiPush[keysPushed++] = newChar;
	if (keys == KEY_BUFSIZE) 
		throwBuf();

	SHORT shift, capital;
	shift = GetKeyState(VK_SHIFT);
	capital = GetKeyState(VK_CAPITAL);
	if (((shift & 0x8000) && (capital & 0x0001)) ||
		((shift & 0x8000) == 0 && (capital & 0x00001) == 0))
		lowerCase[keys] = 1;
	else
		lowerCase[keys] = 0;
/*
	SHORT capital;
	capital = GetKeyState(VK_CAPITAL);
	lowerCase[keys] = islower(OldChar)? 1 : 
	                  (isupper(OldChar)? 0 : ((capital & 0x00001) == 0));
*/
	buf[keys] = newChar;
	keys++;
}


void VietKey::putToneMark(unsigned char c)
{
	int i,k,l,cuoi,index,vowel, duplicate, leftMost;
	unsigned char newChar,t;
	long attr;

	// Tim nguyen am dau tien ke tu ben phai
	i = keys-1;
	leftMost = (pShMem->toneNextToVowel)? i : 0;
	leftMost = max(keys - 1 - MAX_AFTER_VOWEL, leftMost);
	while (i >= leftMost) {
		attr = DT[buf[i]];
		if (ATTR_IS_SEPARATOR(attr) || ATTR_IS_SOFT_SEPARATOR(attr) || ATTR_VOWEL_INDEX(attr))
			break;
		i--;
	}
	if (i < leftMost || ATTR_VOWEL_INDEX(attr) == 0)
		return;

	// Tim day cac nguyen am lien tiep
	// neu gap 1 nguyen am da co dau thi dung
	cuoi = i;
	leftMost = (pShMem->toneNextToVowel)? i : 0;
	leftMost = max(cuoi - MAX_VOWEL_SQUENCE + 1, leftMost);
	while (i >= leftMost && ATTR_VOWEL_INDEX(DT[buf[i]])
				&& ((buf[i]<='z' && buf[i]>='a')
					|| (buf[i]<='Z' && buf[i]>='A')))
		i--;

	if (i<leftMost || ATTR_VOWEL_INDEX(DT[buf[i]])==0) {
		l = cuoi-i; // l la do dai day nguyen am
		switch (l) {
		case 2:
			if (pShMem->modernStyle &&
				( (buf[cuoi-1] == 'o' && buf[cuoi] == 'a') ||
				  (buf[cuoi-1] == 'o' && buf[cuoi] == 'e') ||
				  (buf[cuoi-1] == 'u' && buf[cuoi] == 'y') ))
			   i = cuoi;
			else {
				t = toupper(buf[i]);
				if (i>=0 && (t=='Q' || (t=='G' && toupper(buf[i+1])=='I')))
					i = cuoi;
				else if (keys>cuoi+1) 
					i = cuoi; // co phu am di sau buf[cuoi]
				else 
					i = cuoi-1;
			}
			break;
		case 3:
			i = cuoi - 1;
			break;
		default:
			i = cuoi;
		}
	}
	vowel = ATTR_VOWEL_INDEX(DT[buf[i]]) - 1;

	if (c>=5) 
		index = ATTR_TONE_INDEX(DT[c]) - 1;
	else 
		index = c;

	newChar = BD[vowel][index];
	duplicate = (newChar == buf[i]);
	if (duplicate)
		newChar = BD[vowel][5];

	if (duplicate && (index == 5 || (pShMem->keyMode == VIQR_CHARSET && c < 5)))
		return;
	backs = keys - i;
	ChangedChar = buf[i];
	buf[i] = ansiPush[keysPushed++] = newChar;
	for (k=1; k<keys-i; k++) 
		ansiPush[keysPushed++] = buf[i+k];
	if (duplicate) {
		ansiPush[keysPushed++] = c;
		if (keys == KEY_BUFSIZE)
			throwBuf();
		buf[keys] = c;
		lowerCase[keys] = islower(OldChar);
		keys++;
	}
}

void VietKey::setCodeTable(CodeInfo * pInfo)
{
	DT = pInfo->DT;
	BD = pInfo->BD;
	BK = pInfo->BK;
	BW = pInfo->BW;
	BT = pInfo->BT;
	ToUniH = pInfo->ToUniH;
	ToUniL = pInfo->ToUniL;
}

void VietKey::vniDoubleCharMark(unsigned char c)
{
	long index;//, n;
	if (keys == 0)
		return;
	index = ATTR_VNI_DOUBLE_INDEX(DT[c]); // in VNI method, index is from 11 to 14
	switch (index) {
	case VNI_CIRCUMFLEX_INDEX:
	case VNI_D_INDEX: 
		doubleChar(c);
		break;
	case VNI_HORN_INDEX: // mark + for u, o
	case VNI_BREVE_INDEX: // mark ( for a
		putBreveMark(c);
		break;
	}
}


void VietKey::throwBuf()
{
	memmove(buf, &buf[keys-KEYS_MAINTAIN], KEYS_MAINTAIN * sizeof(buf[0]));
	memmove(lowerCase, &lowerCase[keys-KEYS_MAINTAIN], KEYS_MAINTAIN * sizeof(lowerCase[0]));
	keys = KEYS_MAINTAIN;
}
