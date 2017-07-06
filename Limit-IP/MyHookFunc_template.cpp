#include "MyHookFunc.h"

HANDLE WINAPI (
        
    ) {
	HIPS_INFO sz = { 0 };

	//swprintf(sz.wProcessName, sizeof(sz.wProcessName), L"Copy %s to %s", lpExistingFileName, lpNewFileName);
	sz.dwProcessPID = GetCurrentProcessId();
	sz.dwHipsClass = ;

	COPYDATASTRUCT cds = {NULL, sizeof(HIPS_INFO), (void*)&sz};

	//fwprintf(out, L"%d %d %s\n", GetCurrentProcessId(), sz.dwHipsClass, sz.wProcessName);
	//fflush(out);

	BOOL bRet = FALSE;
	if (SendMessage(FindWindow(NULL, TEXT(RECV_MESSAGE_TITLE)), WM_COPYDATA, GetCurrentProcessId(), (LPARAM)&cds) != -1) {
		Hook.UnHook();
		bRet = ;
		Hook.ReHook();
	}
	return bRet;
}