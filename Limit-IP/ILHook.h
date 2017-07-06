#ifndef __ILHOOK_H_
#define __ILHOOK_H_

#include <windows.h>
#include <cstring>

class CILHook{
public:
    CILHook();
    ~CILHook();
    BOOL Hook(LPCWSTR pszModuleName, LPSTR pszFuncName, PROC pfnHookFunc);
    VOID UnHook();
    BOOL ReHook();

private:
    PROC m_pfnOrig;//函数地址
    BYTE m_bOldBytes[5];//函数入口代码
    BYTE m_bNewBytes[5];//Inline代码
};

#endif