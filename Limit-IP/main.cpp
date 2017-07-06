#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <cstdio>
#include "ILHook.h"
#include "MyHookFunc\MyHookFunc.h"

#pragma data_seg(".shared")
HHOOK g_hHook     = NULL;
HWND  g_ExeHwnd   = NULL;
HINSTANCE g_hInst = NULL;
#pragma data_seg()

//#pragma comment(linker, "/shared, /RWS")

extern "C" __declspec(dllexport) VOID SetHookOn (HWND);
extern "C" __declspec(dllexport) VOID SetHookOff();

LRESULT CALLBACK GetMsgProc(int code, WPARAM wParam, LPARAM lParam) {
	return CallNextHookEx(g_hHook, code, wParam, lParam);
}

VOID SetHookOn(HWND hwnd) {
	g_ExeHwnd = hwnd;
	SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, g_hInst, 0);
}

VOID SetHookOff() {
	UnhookWindowsHookEx(g_hHook);
	g_hHook = NULL;
}

bool ALLOW = true;

CILHook ConnectWHook;
CILHook SendtoWHook;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch(ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			g_hInst = (HINSTANCE)hModule;
			ConnectWHook.Hook(TEXT("Ws2_32.dll"), "connect", (PROC)MyConnect);
			SendtoWHook .Hook(TEXT("Ws2_32.dll"), "sendto" , (PROC)MySendto );
			puts("Hooked!");
			break;
		case DLL_PROCESS_DETACH :
			ConnectWHook.UnHook();
			SendtoWHook .UnHook();
			if (g_hHook != NULL)
				SetHookOff();
			puts("UnHooked!\n");
			break;
	}
	return TRUE;
}
