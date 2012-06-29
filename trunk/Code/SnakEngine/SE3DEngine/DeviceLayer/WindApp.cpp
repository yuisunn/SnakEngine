//windows 时间 实现文件
// 版本
/////////////////////////////////
#include <SE3DEngine/DeviceLayer/WindApp.h>
#pragma comment( lib,"winmm.lib" )
#pragma comment(linker, "/subsystem:console")

namespace SE
{
	//获得句柄
	//////////////////////////
	HWND  GetWindowHandle()
	{
		return m_hwnd;
	}
	//获得窗口实现
	//////////////////////////
	HINSTANCE  WindAppGetWindowInstance()
	{
		return m_hInstance;
	}
	//获得鼠标状态
	//////////////////////////////
	void  GetMouseState(int &x, int&y, int button[3])
	{
		POINT p;

		GetCursorPos(&p);
		ScreenToClient(m_hwnd, &p);

		x = p.x;
		y = p.y;

		button[0] = GetKeyState(VK_LBUTTON & 0x80 ? 1 : 0);
		button[0] = GetKeyState(VK_MBUTTON & 0x80 ? 1 : 0);
		button[0] = GetKeyState(VK_RBUTTON & 0x80 ? 1 : 0);

	}
	//消息循环
	////////////
	bool  ProcessMessageSE()
	{
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}

	//消息循环
	///////////////////
	static LRESULT WINAPI  WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam)
	{
		switch(message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lparam);
			break;
		}
		return 0;
	}

	//创建窗口
	/////////////////////////
	bool  CreateWindowApp(int width, int height, int x, int y, const char *title)
	{
		WNDCLASS window_class;

		memset(&window_class, 0, sizeof(WNDCLASS));

		window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

		window_class.lpfnWndProc = WndProc;

		window_class.hInstance = GetModuleHandle(NULL);

		window_class.hCursor = LoadCursor(NULL, IDC_ARROW);

		window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

		window_class.lpszClassName = title;
	    
		RegisterClass(&window_class);

		DWORD window_style = window_style = WS_BORDER | WS_SYSMENU;

		RECT window_rect;
		SetRect(&window_rect, x, y, x+width, y+height);
		AdjustWindowRect(&window_rect, window_style, FALSE); //该函数依据所需客户矩形的大小，计算需要的窗口矩形的大小
		
		HWND window_handle = CreateWindowEx(
			WS_EX_APPWINDOW,
			title,
			title,
			window_style,
			window_rect.left,
			window_rect.top,
			window_rect.right - window_rect.left,
			window_rect.right - window_rect.top,
			NULL,
			NULL,
			window_class.hInstance,
			NULL
			);

		if(window_handle == NULL)
			return false;

		m_hwnd = window_handle;
		m_hInstance = window_class.hInstance;

		ShowWindow(window_handle, SW_SHOWNORMAL);
		SetActiveWindow(window_handle);
		return true;
	  }
}