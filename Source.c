#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASS wc = {0};
	MSG msg;
	HWND hWnd;
	HANDLE hFile = CreateFile(TEXT("TimeStamp.txt"), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD nBytesWritten = 0;
	SYSTEMTIME Time = {0};
	char TimeStr[] = "00:00";
	TCHAR OSPath[MAX_PATH];
	GetWindowsDirectory(OSPath, MAX_PATH);	/* Get OS Directory into infoBuf */
	
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = TEXT("Sample Window Class");
	wc.lpszMenuName = NULL;
	wc.hInstance = hInstance;
	RegisterClass(&wc);	/* registring wc - a simple defualt window */

	hWnd = CreateWindow(TEXT("Sample Window Class"), TEXT("Sample Window"), WS_OVERLAPPEDWINDOW, 0, 0, 500, 500, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);	/* show the window */

	
	while (GetMessage(&msg, NULL, 0, 0))	
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.wParam == VK_RETURN) {		/* enter key pressed? */
			GetLocalTime(&Time);
			sprintf_s(TimeStr, sizeof(TimeStr), "%d:%d", Time.wHour, Time.wMinute);
			WriteFile(hFile, TimeStr, sizeof(TimeStr), &nBytesWritten, NULL);	/* time log in file "TimeStamp.txt" */
		}
		else if (msg.wParam == VK_SPACE) {	/* space key pressed? */
			MessageBox(hWnd, OSPath, TEXT("OS Path:"), MB_OK);	/* message box with the OS path */
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int count = 0;
	char str[16];

	switch (msg)
	{
	case WM_RBUTTONDOWN:
		++count;	/* each time the right mouse button is clicked -> add 1 to counter */  
		return 0;
		/*break;*/
	case WM_LBUTTONDOWN:
		sprintf_s(str, sizeof(str), "%d", count);
		SetWindowTextA(hWnd, str);	/* set window title to count */
		return 0;
		/*break;*/
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		/*break;*/
	/*case WM_KEYDOWN:
		if (wParam == 13) {		 enter key pressed? 
			
		}
		return 0;
		break;*/
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
};