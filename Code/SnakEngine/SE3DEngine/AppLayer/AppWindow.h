#ifndef _AppWindow_H_
#define _AppWindow_H_

namespace SE
{
	class AppWindow
	{
		HWND GetWindowHandle();
		HINSTANCE GetWindowInstance();
		bool CreateWindowApp(int Width, int height, int x, int y, const char *title);
		static LRESULT WINAPI WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam);
		bool ProcessMessageSE();
		 
		HWND m_hwnd = NULL;
		HINSTANCE m_hInstance = NULL;
	}
}

#endif
