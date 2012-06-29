//windows 窗口消息循环
//版本
///////////////////////////
#ifndef _WIN32_H_
#define _WIN32_H_ 
#include <windows.h>

namespace SE
{
 
		 
		HWND GetWindowHandle();
		HINSTANCE GetWindowInstance();
		bool CreateWindowApp(int Width, int height, int x, int y, const char *title);
		static LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam);
		bool ProcessMessageSE();
		 
		HWND m_hwnd = NULL;
		HINSTANCE m_hInstance = NULL;

 
}
#endif