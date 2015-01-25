#include "prehdr.h"
#include "font.h"

//---------------------------------------------------------------
// call-back function for enumerating fonts
//   lParam: the test function will set this data to font name
//---------------------------------------------------------------
static int FontFound = 0;
int CALLBACK myEnumFontCallback(
					ENUMLOGFONTEX *lpelfe,    // logical-font data
					NEWTEXTMETRICEX *lpntme,  // physical-font data
					DWORD FontType,           // type of font
					LPARAM lParam             // application-defined data
)
{
	if (_tcsicmp((const TCHAR *)lParam, lpelfe->elfLogFont.lfFaceName) == 0)
		FontFound = 1;
	return 0;
}

//----------------------------------------------------------------
int FontExists(const TCHAR *fontFace)
{
	LOGFONT lf;
	HWND hWndDesktop = GetDesktopWindow();
	HDC hDC = GetDC(hWndDesktop);
	FontFound = 0;
	lf.lfCharSet = DEFAULT_CHARSET; //any charset
	_tcscpy(lf.lfFaceName, fontFace);
	lf.lfPitchAndFamily = 0;
	EnumFontFamiliesEx(hDC, &lf, (FONTENUMPROC)myEnumFontCallback, (LPARAM)fontFace, 0);
	ReleaseDC(hWndDesktop, hDC);
	return (FontFound);
}

