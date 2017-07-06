#include "MyHookFunc.h"

/* function name */(
	/* ... */
) {
	SendIPMessage(name);

	int bRet = WSAECONNREFUSED;
	if (IP_check(name)) {
		/**/WHook.UnHook();
		bRet = /**/(s, name, namelen);
		/**/WHook.ReHook();
	}
	return bRet;
}
