#include "MyHookFunc.h"

int WINAPI MySendto(
	_In_       SOCKET                s,
	_In_ const char                  *buf,
	_In_       int                   len,
	_In_       int                   flags,
	_In_       const struct sockaddr *to,
	_In_       int                   tolen
) {
	SendIPMessage(to);

	int bRet = WSAECONNREFUSED;
	if (IP_check(to)) {
		SendtoWHook.UnHook();
		bRet = sendto(s, buf, len, flags, to, tolen);
		SendtoWHook.ReHook();
	}
	return bRet;
}
