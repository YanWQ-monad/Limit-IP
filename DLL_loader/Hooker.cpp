#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <cstdio>
using std::string;

#ifndef _T
# define _T(st) L##st
#endif

typedef VOID (*SetHookOn_ptr_t)(HWND);
struct IP_addr {
	unsigned char a, b, c, d;
	DWORD dwProcessPID;
};

FILE *flog;

LPCWSTR GetProcessNameByPId(const DWORD ProcessID);

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	int ret = 0;
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		case WM_COPYDATA: {
			COPYDATASTRUCT* cds = (COPYDATASTRUCT*)lParam;
			IP_addr* ip = (IP_addr*)cds -> lpData;
			
			fprintf(flog, "%ls visited %d.%d.%d.%d\n", GetProcessNameByPId(ip->dwProcessPID), ip->a, ip->b, ip->c, ip->d);
			printf("%ls visited %d.%d.%d.%d\n", GetProcessNameByPId(ip->dwProcessPID), ip->a, ip->b, ip->c, ip->d);
			
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return ret;
}

wchar_t result[64];
LPCWSTR GetProcessNameByPId(const DWORD ProcessID) {
	HANDLE hProcessSnap;
	PROCESSENTRY32W pe32;
	if (INVALID_HANDLE_VALUE == (hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0))) return L"";

	pe32.dwSize = sizeof(PROCESSENTRY32W);

	if (!Process32FirstW(hProcessSnap, &pe32)) {
		CloseHandle(hProcessSnap);
		return L"";
	}

	do {
		if (ProcessID == pe32.th32ProcessID) {
			wcscpy(result, pe32.szExeFile);
			break;
		}
	} while (Process32NextW(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	MSG Msg; /* A temporary location for all messages */

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = _T("WindowClass");
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, _T("Window Registration Failed!"),_T("Error!"),MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	HWND hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,_T("WindowClass"),_T("Hooker"),WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, _T("Window Creation Failed!"),_T("Error!"),MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	ShowWindow(GetForegroundWindow(), SW_HIDE);

	AllocConsole();
	freopen("CON", "r", stdin );
	freopen("CON", "w", stdout);
	setlocale(LC_CTYPE, "chs");
	
	HINSTANCE hDllInst = LoadLibrary(_T("Limit-IP.dll"));
	SetHookOn_ptr_t SetHookOn = NULL;
	if (hDllInst == NULL)
		if ((hDllInst = LoadLibrary(_T("Limit-IP.dll"))) == NULL) {
			printf("Can't Load DLL\n");
			goto end;
		}
	SetHookOn = (SetHookOn_ptr_t)GetProcAddress(hDllInst, "SetHookOn"); 
	if (SetHookOn == NULL) {
		printf("Can't Load Func \"SetHookOn\"\n");
		goto end;
	}
	SetHookOn(hwnd);
	flog = fopen("log.log", "a");
	
	while(GetMessage(&Msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&Msg); /* Translate key codes to chars if present */
		DispatchMessage(&Msg); /* Send it to WndProc */
		//printf("Hello\n");
	}
	goto end2;
	
	//SetHookOff();
	fclose(flog);
	end:
	getchar();
	end2:
	FreeLibrary(hDllInst);
	FreeConsole();
	
	return Msg.wParam;
}
