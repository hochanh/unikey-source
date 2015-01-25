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
#include "keyhook.h"
#include "encode.h"
#include <stdio.h>
/*
Explanation of encoding in DT table:
In DT:
- Bits 0-4: index of vowels
- Bits 5-8: index of macro keys (e.g. TELEX: w, W, [, ]
- Bits 9-13: index of double characters (including: a, d, e, o, a^, dd, e^, o^, a(, u+...)
- Bits 14-17: index of tone marks
- Bits 18-21: determines the index of the tone associated with a character, e.g tone_index(a') = 1.
- Bit 22: is set if this character is used to type a breve. (in texlex, they are w, W)
- Bit 24: soft separator. Soft separators don't cause the buffer to be cleared.
- Bit 25: separator flag. Buffer must be cleared when separator character is encountered.
- Bit 26-28: index of VNI double marks ('6', '7', '8', '9')
*/


/*/////////////////////////////////////////////////////////////////////

STEPS TO ADD NEW CHARSETS

1. For 1-byte charsets
- Append charset to the end of table MapBD, MapBK, MapBW, MapBT
- Insert charset information into CharsetTable (to any position you like), provide following information:
	+ Charset name
	+ The index of the charset in the table MapBD (the same as in MapBK, MapBW, MapBT)
- Increase TOTAL_CHARSET constnt (in keycons.h) by 1 to reflex the number of supported charsets

2. For 2-byte charsets
- Append charset to the end of BOTH tables UniVnL (for lowercase), UniVnH (for upper case)
- Insert charset information into CharsetTable (to any position you like), provide following information:
	+ Charset name
	+ The index of the charset in the tables UniVnL (the same as in UniVnH) PLUS 0x101
- Increase TOTAL_CHARSET constant by 1 in keycons.h to reflex the number of supported charsets

//////////////////////////////////////////////////////////////////////*/
CharsetInfo CharsetTable[TOTAL_CHARSET] = {
	{_TEXT("TCVN3 (ABC)"),      0},
	{_TEXT("Unicode"),			0x101},
	{_TEXT("VIQR"),				0x100},
	{_TEXT("VNI Windows"),		0x102},
	{_TEXT("VISCII"),			0x106},
	{_TEXT("VPS"),				0x104},
	{_TEXT("BK HCM 2"),			0x103},
	{_TEXT("BK HCM 1"),			0x107},
	{_TEXT("Vietware X"),		0x105},
	{_TEXT("Vietware F"),		2}};

////////////////////////////////////
// 1-byte charsets
////////////////////////////////////
unsigned char MapBD[][12][6] = {
//TCVN 3
    {{'¸','µ','¶','·','¹','a'},		// 0: a
    {'Ê','Ç','È','É','Ë','©'},		// 1: a^
    {'¾','»','¼','½','Æ','¨'},		// 2: a(
    {'Ð','Ì','Î','Ï','Ñ','e'},		// 3: e
    {'Õ','Ò','Ó','Ô','Ö','ª'},		// 4: e^
    {'Ý','×','Ø','Ü','Þ','i'},		// 5: i
    {'ã','ß','á','â','ä','o'},		// 6: o
    {'è','å','æ','ç','é','«'},		// 7: o^
    {'í','ê','ë','ì','î','¬'},		// 8: o+
    {'ó','ï','ñ','ò','ô','u'},		// 9: u
    {'ø','õ','ö','÷','ù','­'},		//10: u+ 
    {'ý','ú','û','ü','þ','y'}},		//11: y
//Vietware-F
    {{'À','ª','¶','º','Á','a'},
    {'Ê','Ç','È','É','Ë','¡'},
    {'Å','Â','Ã','Ä','Æ','Ÿ'},
    {'Ï','Ì','Í','Î','Ñ','e'},
    {'Õ','Ò','Ó','Ô','Ö','£'},
    {'Û','Ø','Ù','Ú','Ü','i'},
    {'â','ß','à','á','ã','o'},
    {'ç','ä','å','æ','è','¤'},
    {'ì','é','ê','ë','í','¥'},
    {'ò','î','ï','ñ','ó','u'},
    {'÷','ô','õ','ö','ø','§'},
    {'ü','ù','ú','û','ÿ','y'}}
};


unsigned char MapBK[][8] = {
	{'®', '§', 'ª', '£', '©', '¢', '«', '¤'},   //TCVN3
	{'¢', '˜', '£', '™', '¡', '—', '¤', 'š'}    //Vietware-F
};


unsigned char MapBW[][6] = {
	{'¨', '¡', '¬', '¥', '­', '¦'},   //TCVNI3
	{'Ÿ', '–', '¥', '›', '§', 'œ'}    //Vietware-F
};

unsigned char MapBT[][6] = {  // this tables must correspond to TelexShortcuts table
					{'¬', '­', '­', '¦', '¥', '¦'},	//TCVN3
					{'¥', '§', '§', 'œ', '¦', '§'}	//Vietware-F
};

//////////////////////////////////////////////////
// Unicode and 2-byte charsets
//////////////////////////////////////////////////

// Lower case
WORD UniVnL[][72] = {
//Unicode
	{0x00e1, 0x00e0, 0x1ea3, 0x00e3, 0x1ea1, 0x0061,
	 0x1ea5, 0x1ea7, 0x1ea9, 0x1eab, 0x1ead, 0x00e2,
	 0x1eaf, 0x1eb1, 0x1eb3, 0x1eb5, 0x1eb7, 0x0103,
	 0x00e9, 0x00e8, 0x1ebb, 0x1ebd, 0x1eb9, 0x0065,
	 0x1ebf, 0x1ec1, 0x1ec3, 0x1ec5, 0x1ec7, 0x00ea,
	 0x00ed, 0x00ec, 0x1ec9, 0x0129, 0x1ecb, 0x0069,
	 0x00f3, 0x00f2, 0x1ecf, 0x00f5, 0x1ecd, 0x006f,
	 0x1ed1, 0x1ed3, 0x1ed5, 0x1ed7, 0x1ed9, 0x00f4,
	 0x1edb, 0x1edd, 0x1edf, 0x1ee1, 0x1ee3, 0x01a1, 
	 0x00fa, 0x00f9, 0x1ee7, 0x0169, 0x1ee5, 0x0075,
	 0x1ee9, 0x1eeb, 0x1eed, 0x1eef, 0x1ef1, 0x01b0, 
	 0x00fd, 0x1ef3, 0x1ef7, 0x1ef9, 0x1ef5, 0x0079},
//VNI
	{0xf961, 0xf861, 0xfb61, 0xf561, 0xef61, 0x0061,
	 0xe161, 0xe061, 0xe561, 0xe361, 0xe461, 0xe261,
	 0xe961, 0xe861, 0xfa61, 0xfc61, 0xeb61, 0xea61,
	 0xf965, 0xf865, 0xfb65, 0xf565, 0xef65, 0x0065,
	 0xe165, 0xe065, 0xe565, 0xe365, 0xe465, 0xe265,
	 0x00ed, 0x00ec, 0x00e6, 0x00f3, 0x00f2, 0x0069,
	 0xf96f, 0xf86f, 0xfb6f, 0xf56f, 0xef6f, 0x006f,
	 0xe16f, 0xe06f, 0xe56f, 0xe36f, 0xe46f, 0xe26f,
	 0xf9f4, 0xf8f4, 0xfbf4, 0xf5f4, 0xeff4, 0x00f4,
	 0xf975, 0xf875, 0xfb75, 0xf575, 0xef75, 0x0075,
	 0xf9f6, 0xf8f6, 0xfbf6, 0xf5f6, 0xeff6, 0x00f6,
	 0xf979, 0xf879, 0xfb79, 0xf579, 0x00ee, 0x0079},
//BKHCM2
	{0xe161, 0xe261, 0xe361, 0xe461, 0xe561, 0x0061,
	 0xebea, 0xecea, 0xEDEA, 0xEEEA, 0xE5EA, 0x00EA,
	 0xE6F9, 0xE7F9, 0xE8F9, 0xE9F9, 0xE5F9, 0x00F9,
	 0xE165, 0xE265, 0xE365, 0xE465, 0xE565, 0x0065,
	 0xEBEF, 0xECEF, 0xEDEF, 0xEEEF, 0xE5EF, 0x00EF,
	 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x0069,
	 0xE16F, 0xE26F, 0xE36F, 0xE46F, 0xE56F, 0x006F,
	 0xEBF6, 0xECF6, 0xEDF6, 0xEEF6, 0xE5F6, 0x00F6,
	 0xE1FA, 0xE2FA, 0xE3FA, 0xE4FA, 0xE5FA, 0x00FA,
	 0xE175, 0xE275, 0xE375, 0xE475, 0xE575, 0x0075,
	 0xE1FB, 0xE2FB, 0xE3FB, 0xE4FB, 0xE5FB, 0x00FB,
	 0xE179, 0xE279, 0xE379, 0xE479, 0xE579, 0x0079},
//VPS
	{0x00E1, 0x00E0, 0x00E4, 0x00E3, 0x00E5, 0x0061,
	 0x00C3, 0x00C0, 0x00C4, 0x00C5, 0x00C6, 0x00E2,
	 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00E6,
	 0x00E9, 0x00E8, 0x00C8, 0x00EB, 0x00CB, 0x0065,
	 0x0089, 0x008A, 0x008B, 0x00CD, 0x008C, 0x00EA,
	 0x00ED, 0x00EC, 0x00CC, 0x00EF, 0x00CE, 0x0069,
	 0x00F3, 0x00F2, 0x00D5, 0x00F5, 0x0086, 0x006F,
	 0x00D3, 0x00D2, 0x00B0, 0x0087, 0x00B6, 0x00F4,
	 0x00A7, 0x00A9, 0x00AA, 0x00AB, 0x00AE, 0x00D6,
	 0x00FA, 0x00F9, 0x00FB, 0x00DB, 0x00F8, 0x0075,
	 0x00D9, 0x00D8, 0x00BA, 0x00BB, 0x00BF, 0x00DC,
	 0x009A, 0x00FF, 0x009B, 0x00CF, 0x009C, 0x0079},
//VIETWARE X
	{0xEF61, 0xEC61, 0xED61, 0xEE61, 0xFB61, 0x0061,
	 0xFAE1, 0xF6E1, 0xF8E1, 0xF9E1, 0xFBE1, 0x00E1,
	 0xF5E0, 0xF2E0, 0xF3E0, 0xF4E0, 0xFBE0, 0x00E0,
	 0xEF65, 0xEC65, 0xED65, 0xEE65, 0xFB65, 0x0065,
	 0xFAE3, 0xF6E3, 0xF8E3, 0xF9E3, 0xFBE3, 0x00E3,
	 0x00EA, 0x00E7, 0x00E8, 0x00E9, 0x00EB, 0x0069,
	 0xEF6F, 0xEC6F, 0xED6F, 0xEE6F, 0xFC6F, 0x006F,
	 0xFAE4, 0xF6E4, 0xF8E4, 0xF9E4, 0xFCE4, 0x00E4,
	 0xEFE5, 0xECE5, 0xEDE5, 0xEEE5, 0xFCE5, 0x00E5,
	 0xEF75, 0xEC75, 0xED75, 0xEE75, 0xFB75, 0x0075,
	 0xEFE6, 0xECE6, 0xEDE6, 0xEEE6, 0xFBE6, 0x00E6,
	 0xEF79, 0xEC79, 0xED79, 0xEE79, 0xF179, 0x0079},
//VISCII
	{0x00E1, 0x00E0, 0x00E4, 0x00E3, 0x00D5, 0x0061,
	 0x00A4, 0x00A5, 0x00A6, 0x00E7, 0x00A7, 0x00E2,
	 0x00A1, 0x00A2, 0x00C6, 0x00C7, 0x00A3, 0x00E5,
	 0x00E9, 0x00E8, 0x00EB, 0x00A8, 0x00A9, 0x0065,
	 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00EA,
	 0x00ED, 0x00EC, 0x00EF, 0x00EE, 0x00B8, 0x0069,
	 0x00F3, 0x00F2, 0x00F6, 0x00F5, 0x00F7, 0x006F,
	 0x00AF, 0x00B0, 0x00B1, 0x00B2, 0x00B5, 0x00F4,
	 0x00BE, 0x00B6, 0x00B7, 0x00DE, 0x00FE, 0x00BD,
	 0x00FA, 0x00F9, 0x00FC, 0x00FB, 0x00F8, 0x0075,
	 0x00D1, 0x00D7, 0x00D8, 0x00E6, 0x00F1, 0x00DF,
	 0x00FD, 0x00CF, 0x00D6, 0x00DB, 0x00DC, 0x0079},
//BKHCM1
	{0x00BE, 0x00BF, 0x00C0, 0x00C1, 0x00C2, 0x0061,
	 0x00DE, 0x00DF, 0x00E0, 0x00E1, 0x00E2, 0x00DD,
	 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00D7,
	 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x0065,
	 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E3,
	 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x0069,
	 0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x006F,
	 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00E9,
	 0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00EF,
	 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x0075,
	 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00F5,
	 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF, 0x0079},
};

// Uppercase
WORD UniVnH[][72] = {
// Unicode
	{0x00c1, 0x00c0, 0x1ea2, 0x00c3, 0x1ea0, 0x0041,
	 0x1ea4, 0x1ea6, 0x1ea8, 0x1eaa, 0x1eac, 0x00c2,
	 0x1eae, 0x1eb0, 0x1eb2, 0x1eb4, 0x1eb6, 0x0102,
	 0x00c9, 0x00c8, 0x1eba, 0x1ebc, 0x1eb8, 0x0045,
	 0x1ebe, 0x1ec0, 0x1ec2, 0x1ec4, 0x1ec6, 0x00ca,
	 0x00cd, 0x00cc, 0x1ec8, 0x0128, 0x1eca, 0x0049,
	 0x00d3, 0x00d2, 0x1ece, 0x00d5, 0x1ecc, 0x004f,
	 0x1ed0, 0x1ed2, 0x1ed4, 0x1ed6, 0x1ed8, 0x00d4,
	 0x1eda, 0x1edc, 0x1ede, 0x1ee0, 0x1ee2, 0x01a0,
	 0x00da, 0x00d9, 0x1ee6, 0x0168, 0x1ee4, 0x0055,
	 0x1ee8, 0x1eea, 0x1eec, 0x1eee, 0x1ef0, 0x01af,
	 0x00dd, 0x1ef2, 0x1ef6, 0x1ef8, 0x1ef4, 0x0059},
// VNI
	{0xd941, 0xd841, 0xdb41, 0xd541, 0xcf41, 0x0041,
	 0xc141, 0xc041, 0xc541, 0xc341, 0xc441, 0xc241,
	 0xc941, 0xc841, 0xda41, 0xdc41, 0xcb41, 0xca41,
	 0xd945, 0xd845, 0xdb45, 0xd545, 0xcf45, 0x0045,
	 0xc145, 0xc045, 0xc545, 0xc345, 0xc445, 0xc245,
	 0x00cd, 0x00cc, 0x00c6, 0x00d3, 0x00d2, 0x0049,
	 0xd94f, 0xd84f, 0xdb4f, 0xd54f, 0xcf4f, 0x004f,
	 0xc14f, 0xc04f, 0xc54f, 0xc34f, 0xc44f, 0xc24f,
	 0xd9d4, 0xd8d4, 0xdbd4, 0xd5d4, 0xcfd4, 0x00d4,
	 0xd955, 0xd855, 0xdb55, 0xd555, 0xcf55, 0x0055,
	 0xd9d6, 0xd8d6, 0xdbd6, 0xd5d6, 0xcfd6, 0x00d6,
	 0xd959, 0xd859, 0xdb59, 0xd559, 0x00ce, 0x0059},
//BKHCM2
	{0xC141, 0xC241, 0xC341, 0xC441, 0xC541, 0x0041,
	 0xCBCA, 0xCCCA, 0xCDCA, 0xCECA, 0xC5CA, 0x00CA,
	 0xC6D9, 0xC7D9, 0xC8D9, 0xC9D9, 0xC5D9, 0x00D9,
	 0xC145, 0xC245, 0xC345, 0xC445, 0xC545, 0x0045,
	 0xCBCF, 0xCCCF, 0xCDCF, 0xCECF, 0xE5CF, 0x00CF,
	 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x0049,
	 0xC14F, 0xC24F, 0xC34F, 0xC44F, 0xC54F, 0x004F,
	 0xCBD6, 0xCCD6, 0xCDD6, 0xCED6, 0xC5D6, 0x00D6,
	 0xC1DA, 0xC2DA, 0xC3DA, 0xC4DA, 0xC5DA, 0x00DA,
	 0xC155, 0xC255, 0xC355, 0xC455, 0xC555, 0x0055,
	 0xC1DB, 0xC2DB, 0xC3DB, 0xC4DB, 0xC5DB, 0x00DB,
	 0xC159, 0xC259, 0xC359, 0xC459, 0xC559, 0x0059},
//VPS
	{0x00C1, 0x0080, 0x0081, 0x0082, 0x2E41, 0x0041,
	 0x0083, 0x0084, 0x0085, 0x7EC2, 0x2EC2, 0x00C2,
	 0x008D, 0x008E, 0x008F, 0x00F0, 0x2E88, 0x002E,
	 0x00C9, 0x00D7, 0x00DE, 0x00FE, 0x2E45, 0x0045,
	 0x0090, 0x0093, 0x0094, 0x0095, 0x2ECA, 0x00CA,
	 0x00B4, 0x00B5, 0x00B7, 0x00B8, 0x2E49, 0x0049,
	 0x00B9, 0x00BC, 0x00BD, 0x00BE, 0x2E4F, 0x004F,
	 0x0096, 0x0097, 0x0098, 0x0099, 0x2ED4, 0x00D4,
	 0x009D, 0x009E, 0x009F, 0x00A6, 0x2EF7, 0x00F7,
	 0x00DA, 0x00A8, 0x00D1, 0x00AC, 0x2E55, 0x0055,
	 0x00AD, 0x00AF, 0x00B1, 0x7ED0, 0x2ED0, 0x00D0,
	 0x00DD, 0x00B2, 0x00FD, 0x00B3, 0x2E59, 0x0059},
//VIETWARE X
	{0xCF41, 0xCC41, 0xCD41, 0xCE41, 0xDB41, 0x0041,
	 0xDAC1, 0xD6C1, 0xD8C1, 0xD9C1, 0xDBC1, 0x00C1,
	 0xD5C0, 0xD2C0, 0xD3C0, 0xD4C0, 0xDBC0, 0x00C0,
	 0xCF45, 0xCC45, 0xCD45, 0xCE45, 0xDB45, 0x0045,
	 0xDAC3, 0xD6C3, 0xD8C3, 0xD9C3, 0xDBC3, 0x00C3,
	 0x00CA, 0x00C7, 0x00C8, 0x00C9, 0x00CB, 0x0049,
	 0xCF4F, 0xCC4F, 0xCD4F, 0xCE4F, 0xDC4F, 0x004F,
	 0xDAC4, 0xD6C4, 0xD8C4, 0xD9C4, 0xDCC4, 0x00C4,
	 0xCFC5, 0xCCC5, 0xCDC5, 0xCEC5, 0xDCC5, 0x00C5,
	 0xCF55, 0xCC55, 0xCD55, 0xCE55, 0xDB55, 0x0055,
	 0xCFC6, 0xCCC6, 0xCDC6, 0xCEC6, 0xDBC6, 0x00C6,
	 0xCF59, 0xCC59, 0xCD59, 0xCE59, 0xD159, 0x0059},
//VISCII
	{0x00C1, 0x00C0, 0x00C4, 0x00C3, 0x0080, 0x0041,
	 0x0084, 0x0085, 0x0086, 0x00E7, 0x0087, 0x00C2,
	 0x0081, 0x0082, 0x00C6, 0x00C7, 0x0083, 0x00C5,
	 0x00C9, 0x00C8, 0x00CB, 0x0088, 0x0089, 0x0045,
	 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x00CA,
	 0x00CD, 0x00CC, 0x009B, 0x00CE, 0x0098, 0x0049,
	 0x00D3, 0x00D2, 0x0099, 0x00F5, 0x009A, 0x004F,
	 0x008F, 0x0090, 0x0091, 0x0092, 0x0093, 0x00D4,
	 0x0095, 0x0096, 0x0097, 0x00B3, 0x0094, 0x00B4,
	 0x00DA, 0x00D9, 0x009C, 0x009D, 0x009E, 0x0055,
	 0x00BA, 0x00BB, 0x00BC, 0x00FF, 0x00B9, 0x00BF,
	 0x00DD, 0x009F, 0x00D6, 0x00DB, 0x00DC, 0x0059},
//BKHCM1
	{0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0041,
	 0x007E, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x009F,
	 0x009A, 0x009B, 0x009C, 0x009D, 0x0098, 0x0099,
	 0x0085, 0x0086, 0x0087, 0x0088, 0x0089, 0x0045,
	 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00A5,
	 0x008A, 0x008B, 0x008C, 0x008D, 0x008E, 0x0049,
	 0x008F, 0x0090, 0x0091, 0x0092, 0x0093, 0x004F,
	 0x00AC, 0x00AD, 0x00AE, 0x00AF, 0x00B0, 0x00AB,
	 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B1,
	 0x0094, 0x0095, 0x0096, 0x0097, 0x0098, 0x0055,
	 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00B7,
	 0x007B, 0x005E, 0x0060, 0x007C, 0x008E, 0x0059}
};


// Character d-
WORD UniVndd[] = {
					0x0111,   // Unicode
					0x00f1,   // VNI
					0x00E0,   // BKHCM2
					0x00C7,   // VPS
					0x00E2,   // VIETWARE X
					0x00F0,   // VISCII
					0x00BD};  // BKHCM1


// Character -D
WORD UniVnDD[] = {
					0x0110,  // Unicode
					0x00d1,  // VNI
					0x00C0,  // BKHCM2
					0x00F1,  // VPS
					0x00C2,  // VIETWARE X
					0x00D0,  // VISCII
					0x007D}; // BKHCM1

///// End of charset specification ///////////////////
//////////////////////////////////////////////////////

// Base characters of "doule" characters (like a^, a(, u+ )
// the order here must not be changed
unsigned char DoubleChars[] = {'d', 'D', 'e', 'E', 'a', 'A', 'o', 'O', 'u', 'U'};

unsigned char TelexTones[6] = {'s', 'f', 'r', 'x', 'j', 'z'};
unsigned char TelexBreves[] = {'w', 'W'};					// Da^'u tra(ng (breve mark)
unsigned char TelexShortcuts[] = {'[', ']', 'w', 'W', '{', '}'};

unsigned char ToneLimits[] = {
	'b', 'd', 'f', 'j', 'k', 'l', 'q', 'r', 's', 'v', 'w', 'x', 'z',
	'B', 'D', 'F', 'J', 'K', 'L', 'Q', 'R', 'S', 'V', 'W', 'X', 'Z', 
	',', ';', ':', '.', '\"', '\'', '!', '?', 
	' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'<', '>', '=', '+', '-', '*', '/', '\\',
	'_', '~', '`', '@', '#', '$', '%', '^', '&', '(', ')', '{', '}', '[', ']'};


unsigned char VniTones[] = {'1', '2', '3', '4', '5', '0'};
unsigned char VniDoubleMarks[] = {'6', '7', '8', '9'};



unsigned char VIQRtones[] = {'\'','`','?','~','.', 0};
unsigned char VIQRcircumflex = '^';
unsigned char VIQRbreve = '(';
unsigned char VIQRhorn = '+';



void BuildTelexMethod(CodeInfo *pTab);
void BuildVniMethod(CodeInfo *pTab);

void BuildInputMethod(int method, CodeInfo *pTab)
{
	if (method == TELEX_INPUT)
		BuildTelexMethod(pTab);
	else
		BuildVniMethod(pTab);
}

void BuildTelexMethod(CodeInfo *pTab)
{
	long i, j, offset;
	unsigned char ch;

	for (i=0; i<256; i++)
		pTab->DT[i] = 0x2000000; // bit 25: hard separator

	for (ch = 'a'; ch <= 'z'; ch++)
		pTab->DT[ch] = 0;

	for (ch = 'A'; ch <= 'Z'; ch++)
		pTab->DT[ch] = 0;


	for (i = 0; i < sizeof(ToneLimits); i++)
		pTab->DT[ToneLimits[i]] = 0;

	for (i=0; i<12; i++) { // we have 12 vowel sets
		for (j=0; j<6; j++) {// each vowel set has 6
			pTab->DT[pTab->BD[i][j]] |= i+1;
			pTab->DT[pTab->BD[i][j]] |= (j+1) << 18; // tone index (from bit 18)
		}
	}

	// Set index for for characters like: dd, or a(, 
	offset = sizeof(DoubleChars) - sizeof(pTab->BW);
	for (i=0; i < sizeof(DoubleChars); i++) {
		pTab->DT[DoubleChars[i]] |= ((i+1) << 9);
		if (i < sizeof(pTab->BK))
			pTab->DT[pTab->BK[i]] |= ((i+1) << 9);
		if (i+1 > offset)
			pTab->DT[pTab->BW[i-offset]] |= ((i+1) << 9);
	}

	// Set tone limits (soft separator)
	for (i = 0; i < sizeof(ToneLimits); i++)
		pTab->DT[ToneLimits[i]] |= 0x1000000;  // Set bit 24 for soft separators

	// For tone makrs
	for (i=0; i < sizeof(TelexTones); i++) {
		pTab->DT[TelexTones[i]] |= (i+1) << 14; // tone mark from bit 14 to 17
		pTab->DT[toupper(TelexTones[i])] |= pTab->DT[TelexTones[i]];
	}

	// For the mark ( or +
	for (i=0; i < sizeof(TelexBreves); i++)
		pTab->DT[TelexBreves[i]] |= 0x400000; // set bit 22 for the breve mark

	// For macro keys
	for (i=0; i < sizeof(TelexShortcuts); i++)
		pTab->DT[TelexShortcuts[i]] |= (i+1) << 5; // macro keys from bit 5
}

//-------------------------------------------
void BuildVniMethod(CodeInfo *pTab)
{
	int i, j;
	unsigned char ch;
	int offset;

	for (i=0; i<256; i++)
		pTab->DT[i] = 0x2000000; // bit 25: hard separator

	for (ch = 'a'; ch <= 'z'; ch++)
		pTab->DT[ch] = 0;

	for (ch = 'A'; ch <= 'Z'; ch++)
		pTab->DT[ch] = 0;

	for (i = 0; i < sizeof(ToneLimits); i++)
		pTab->DT[ToneLimits[i]] = 0;


	for (i=0; i<12; i++) { // we have 12 vowel sets
		for (j=0; j<6; j++) {
			// each vowel set has 6
			pTab->DT[pTab->BD[i][j]] |= i+1;
			pTab->DT[pTab->BD[i][j]] |= (j+1) << 18; // tone index (from bit 18)
		}
	}

	// For characters like: dd, or a(, 
	offset = sizeof(DoubleChars) - sizeof(pTab->BW);
	for (i=0; i < sizeof(DoubleChars); i++) {
		pTab->DT[DoubleChars[i]] |= ((i+1) << 9);
		if (i < sizeof(pTab->BK))
			pTab->DT[pTab->BK[i]] |= ((i+1) << 9);
		if (i+1 > offset)
			pTab->DT[pTab->BW[i-offset]] |= ((i+1) << 9);
	}

	// Set tone limits
	for (i = 0; i < sizeof(ToneLimits); i++)
		pTab->DT[ToneLimits[i]] |= 0x1000000;  // Set bit 24 for soft separator

	for (j=0; j < sizeof(VniDoubleMarks); j++)
		pTab->DT[VniDoubleMarks[j]] |= (j + 1) << 26; // VNI double marks: from bit 26 - 28

	// For tone makrs
	for (i=0; i < sizeof(VniTones); i++)
		pTab->DT[VniTones[i]] |= (i+1) << 14;  // tone marks from bit 14
}


//---------------------------------------------
void BuildCodeTable(WORD charSet, int inputMethod, CodeInfo *pTab)
{
	int index;
	WORD i, j;
	unsigned char ch, up;

	index = charSet;
	if (charSet >= 0x100) {
		index = TCVN3_CHARSET;
		pTab->multiBytes = 1;
	}
	else pTab->multiBytes = 0;

	memcpy(pTab->BD, MapBD[index], sizeof(pTab->BD));
	memcpy(pTab->BK, MapBK[index], sizeof(pTab->BK));
	memcpy(pTab->BT, MapBT[index], sizeof(pTab->BT));
	memcpy(pTab->BW, MapBW[index], sizeof(pTab->BW));

	BuildInputMethod(inputMethod, pTab);
	//------- Build conversion table from TCVN3 to 2-bytes charsets -------
	if (charSet == VIQR_CHARSET) {
		for (i = 0; i < 256; i++) {
			pTab->ToUniL[i] = i;
			pTab->ToUniH[i] = i;
		}
		for (ch= 'a', up = 'A'; ch <= 'z'; ch++, up++)
			pTab->ToUniH[ch] = up;
		for (i=0; i<12; i++) { // we have 12 vowel sets
			for (j=0; j<6; j++) {
				pTab->ToUniL[pTab->BD[i][j]] = MAKEWORD(pTab->BD[i][5], VIQRtones[j]);
				pTab->ToUniH[pTab->BD[i][j]] = MAKEWORD(toupper(pTab->BD[i][5]), VIQRtones[j]);
			}
		}

		pTab->ToUniL[pTab->BK[0]] = MAKEWORD('d', 'd');
		pTab->ToUniH[pTab->BK[0]] = MAKEWORD('D', 'D');
		for (i = 1; i < sizeof(pTab->BK); i++) {
			pTab->ToUniL[pTab->BK[i]] = MAKEWORD(DoubleChars[i], VIQRcircumflex);
			pTab->ToUniH[pTab->BK[i]] = MAKEWORD(toupper(DoubleChars[i]), VIQRcircumflex);
		}

		int offset = sizeof(DoubleChars) - sizeof(pTab->BW);
		for (i = 0; i < sizeof(pTab->BW); i++) {
			pTab->ToUniL[pTab->BW[i]] = MAKEWORD(DoubleChars[i+offset], (i < 2)? VIQRbreve : VIQRhorn);
			pTab->ToUniH[pTab->BW[i]] = MAKEWORD(toupper(DoubleChars[i+offset]), (i < 2)? VIQRbreve : VIQRhorn);
		}
	}
	else if (pTab->multiBytes) {
		index = charSet - 0x100 - 1; // the first index is assigned to VIQR
		for (i=0; i<256; i++) {
			pTab->ToUniH[i] = i;
			pTab->ToUniL[i] = i;
		}

		for (ch= 'a', up = 'A'; ch <= 'z'; ch++, up++)
			pTab->ToUniH[ch] = up;

		unsigned char *p = (unsigned char *)pTab->BD;
		for (i=0; i<72; i++) {
			pTab->ToUniH[*p] = UniVnH[index][i];
			pTab->ToUniL[*p] = UniVnL[index][i];
			p++;
		}
		pTab->ToUniL[pTab->BK[0]] = UniVndd[index];
		pTab->ToUniH[pTab->BK[0]] = UniVnDD[index];

		pTab->ToUniL[pTab->BT[4]] = UniVnL[index][8*6+5]; // upper O+ (= BD[8][5])
		pTab->ToUniH[pTab->BT[4]] = UniVnH[index][8*6+5];

		pTab->ToUniL[pTab->BT[5]] = UniVnL[index][10*6+5]; // upper U+ (= BD[10][5])
		pTab->ToUniH[pTab->BT[5]] = UniVnH[index][10*6+5];
	}
}

