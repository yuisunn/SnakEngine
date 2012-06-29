//windows ʱ�� ʵ���ļ�
// �汾
/////////////////////////////////
#include <SE3DEngine/DeviceLayer/WindApp.h>
#pragma comment( lib,"winmm.lib" )
#pragma comment(linker, "/subsystem:console")

namespace SE
{
	//��þ��
	//////////////////////////
	HWND  GetWindowHandle()
	{
		return m_hwnd;
	}
	//��ô���ʵ��
	//////////////////////////
	HINSTANCE  WindAppGetWindowInstance()
	{
		return m_hInstance;
	}
	//������״̬
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
	//��Ϣѭ��
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

	//��Ϣѭ��
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

	//��������
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
		AdjustWindowRect(&window_rect, window_style, FALSE); //�ú�����������ͻ����εĴ�С��������Ҫ�Ĵ��ھ��εĴ�С
		
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