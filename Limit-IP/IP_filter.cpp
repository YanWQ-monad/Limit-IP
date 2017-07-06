#include <windows.h>
#include <cstdio>
#include "MyHookFunc\MyHookFunc.h"

bool IP_check(const sockaddr* addr) {
	in_addr ip_addr = ((sockaddr_in*)addr)->sin_addr;
	int a, b, c, d;
	sscanf(inet_ntoa(ip_addr), "%d.%d.%d.%d", &a, &b, &c, &d);

	bool can = (a == 121 && b == 9 && c == 250 && d == 16 ) ||
	           (a == 10  && b == 3 && c == 35  && d == 134) ||
	           (a == 127 && b == 0 && c == 0   && d == 1  );

	return can;
}

void SendIPMessage(const sockaddr *addr) {
	IP_addr ip = { 0, 0, 0, 0 };

	in_addr ip_addr = ((sockaddr_in*)addr)->sin_addr;
	sscanf(inet_ntoa(ip_addr), "%hhu.%hhu.%hhu.%hhu", &ip.a, &ip.b, &ip.c, &ip.d);
	ip.dwProcessPID = GetCurrentProcessId();

	COPYDATASTRUCT cds = { NULL, (DWORD32)sizeof(IP_addr), (void*)&ip };
	SendMessage(FindWindow(NULL, TEXT(RECV_MESSAGE_TITLE)), WM_COPYDATA, GetCurrentProcessId(), (LPARAM)&cds);
}
