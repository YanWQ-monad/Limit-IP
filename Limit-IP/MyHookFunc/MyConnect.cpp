#include "MyHookFunc.h"

int WINAPI MyConnect(
	_In_ SOCKET                s,
	_In_ const struct sockaddr *name,
	_In_ int                   namelen
) {
	SendIPMessage(name);

	int bRet = WSAECONNREFUSED;
	if (IP_check(name)) {
		ConnectWHook.UnHook();
		bRet = connect(s, name, namelen);
		ConnectWHook.ReHook();
	}
	return bRet;
}
