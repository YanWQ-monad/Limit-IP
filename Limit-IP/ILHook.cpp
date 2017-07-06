#include "ILHook.h"

CILHook::CILHook(){
    m_pfnOrig=NULL;
    ZeroMemory(m_bOldBytes, 5);
    ZeroMemory(m_bNewBytes, 5);
}
CILHook::~CILHook(){
    UnHook();
}

/*
函数名称：Hook
函数功能：对指定模块中的函数进行挂钩
参数说明�?
    pszModuleName：模块名�?
    pszFuncName：函数名�?
    pfnHookFunc：钩子函�?
*/
BOOL CILHook::Hook(LPCWSTR pszModuleName,LPSTR pszFuncName,PROC pfnHookFunc){
    BOOL bRet=FALSE;
    
    m_pfnOrig=(PROC)GetProcAddress(GetModuleHandle(pszModuleName),pszFuncName);

    if(m_pfnOrig!=NULL){
        
        DWORD dwNum=0;
        ReadProcessMemory(GetCurrentProcess(),m_pfnOrig,m_bOldBytes,5,&dwNum);
        
        m_bNewBytes[0]='\xe9';
        
        
        
        *(DWORD*)(m_bNewBytes+1)=(DWORD)pfnHookFunc-(DWORD)m_pfnOrig-5;
        
        WriteProcessMemory(GetCurrentProcess(),m_pfnOrig,m_bNewBytes,5,&dwNum);

        bRet=TRUE;
    }
    return bRet;
}

/*
函数名称：UnHook
函数功能：取消函数的挂钩
*/
VOID CILHook::UnHook(){
    if(m_pfnOrig!=0){
        DWORD dwNum=0;
        WriteProcessMemory(GetCurrentProcess(),m_pfnOrig,m_bOldBytes,5,&dwNum);
    }
}

/*
函数名称：ReHook
函数功能：重新对函数进行挂钩
*/
BOOL CILHook::ReHook(){
    BOOL bRet=FALSE;
    if(m_pfnOrig!=0){
        DWORD dwNum=0;
        WriteProcessMemory(GetCurrentProcess(),m_pfnOrig,m_bNewBytes,5,&dwNum);
        bRet=TRUE;
    }
    return bRet;
}