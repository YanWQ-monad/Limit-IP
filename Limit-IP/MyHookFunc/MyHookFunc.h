#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include "..\ILHook.h"
// #include <Winsock2.h>
#include <cstdio>

#pragma data_seg(".shared")
extern HHOOK     g_hHook   ;
extern HWND      g_ExeHwnd ;
extern HINSTANCE g_hInst   ;
#pragma data_seg()

struct IP_addr {
	unsigned char a, b, c, d;
	DWORD dwProcessPID;
};

extern bool ALLOW;
bool IP_check(const sockaddr* addr);      // IP_filter.cpp
void SendIPMessage(const sockaddr *addr); // IP_filter.cpp

#define HIPS_CONNECT 0x00000001L

#define RECV_MESSAGE_TITLE "Hooker"

// extern CILHook (Function name);
extern CILHook ConnectWHook;

int WINAPI MyConnect(
	_In_ SOCKET                s,
	_In_ const struct sockaddr *name,
	_In_ int                   namelen
);
