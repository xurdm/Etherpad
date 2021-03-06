#include <windows.h>
#include <string.h>
#include <direct.h>
#include <wchar.h>
#include "etherpad.h"

int not_main(int argc, char **argv)
{
	LPWSTR data;

	_wfopen_s(&flog, L"console.log", L"w");

	if(data = get_clipboard_data())
	{
		fprintf(flog, "%S\r\n", data);
	}
	else
	{
		fprintf(flog, "Error. I don't know, dude.\r\n");
	}

	fclose(flog);
	return 0;
}

int set_clipboard_data(LPWSTR data)
{
	LPWSTR buffer = data;
	DWORD len;
	HGLOBAL hDest;
	LPWSTR dest;

	len = wcslen(buffer);
	hDest = GlobalAlloc(GMEM_MOVEABLE, (len + 1) * sizeof(WCHAR));
	dest = (LPWSTR)GlobalLock(hDest);
	memcpy(dest, buffer, len * sizeof(WCHAR));
	dest[len] = 0;
	GlobalUnlock(hDest);

	if(!OpenClipboard(NULL))
		return GetLastError();
	EmptyClipboard();
	if(!SetClipboardData(CF_UNICODETEXT, hDest))
		return GetLastError();
	CloseClipboard();

	return 0;
}

LPWSTR get_clipboard_data(void)
{
	HGLOBAL hGlobal;
	LPWSTR lpwstr;
	LPWSTR ret = NULL;

	if(!IsClipboardFormatAvailable(CF_UNICODETEXT))
		//TODO: handle all clipboard formats
		//TODO: care
		return ret;
	if(!OpenClipboard(NULL))
		return ret;
	if(!(hGlobal = GetClipboardData(CF_UNICODETEXT)))
		//should I call CloseClipboard() here?
		return ret;	
	if((lpwstr = (LPWSTR)GlobalLock(hGlobal)) != NULL)
	{
		ret = lpwstr;
		GlobalUnlock(hGlobal);
	}

	CloseClipboard();
	return ret;
}