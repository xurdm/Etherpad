#include <windows.h>
#include <string.h>
#include <direct.h>

int main()
{
	LPWSTR buffer;	//current working directory buffer

	if((buffer = _wgetcwd(NULL, 0)) == NULL)
		return 1;

	DWORD len = wcslen(buffer);
	HGLOBAL hDest;
	LPWSTR dest;

	//current working directory string
	hDest = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (len + 1) * sizeof(WCHAR));
	dest = (LPWSTR)GlobalLock(hDest);
	memcpy(dest, buffer, len * sizeof(WCHAR));
	dest[len] = 0;
	GlobalUnlock(hDest);

	//set clipboard data
	if(!OpenClipboard(NULL))
		return GetLastError();
	EmptyClipboard();
	if(!SetClipboardData(CF_UNICODETEXT, hDest))
		return GetLastError();
	CloseClipboard();

	free(buffer);
	return 0;
}