#include "MyHookFunc.h"

BOOL WINAPI MyCopyFileW(
        __in LPCWSTR lpExistingFileName,
        __in LPCWSTR lpNewFileName,
        __in BOOL    bFailIfExists
	) {

	//fwprintf(out, L"%d %d %s\n", GetCurrentProcessId(), sz.dwHipsClass, sz.wProcessName);
	//fflush(out);

	BOOL bRet = FALSE;
	if (TRUE) {
		CopyFileWHook.UnHook();
		bRet = CopyFileW(lpExistingFileName, lpNewFileName, bFailIfExists);
		CopyFileWHook.ReHook();
	}
	return bRet;
}
