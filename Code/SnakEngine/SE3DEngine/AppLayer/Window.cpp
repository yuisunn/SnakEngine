#include <SE3DEngine/AppLayer/Window.h>


namespace SE
{
#ifdef SE_WINDOWS
	LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		//兼容64 和32 位win用
		//这个函数是在指定的窗口中获取信息。也可以在指定window内存偏移量的情况下获取值。
		Window* win = reinterpret_cast<Window*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));
		if(win != NULL)
		{
			return win->MsgProc(hWnd,uMsg,wParam,lParam);
		}
		else
		{
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	Window::Window(std::string const& name, SRenderConfig const& settings)
	{
		//获得句柄
		HINSTANCE hInst = ::GetModuleHandle(NULL);
		//注册window
#ifdef COMPILER_GCC
		m_sName = name;
		WNDCLASSEXA wc;
#else
		Convert(m_wName, name);
		WNDCLASSEXW wc;
#endif	
		wc.cbSize			= sizeof(wc);
		wc.style			= CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= WndProc;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= sizeof(this);
		wc.hInstance		= hInst;
		wc.hIcon			= NULL;
		wc.hCursor			= ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground	= static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
		wc.lpszMenuName		= NULL;
#ifdef COMPILER_GCC
		wc.lpszClassName	= m_sName.c_str();
#else
		wc.lpszClassName	= m_wsName.c_str();
#endif
        wc.hIconSm			= NULL;
#ifdef COMPILER_GCC
		::RegisterClassExA(&wc);
#else
		::RegisterClassExW(&wc);
#endif
		//窗口样式
		uint32 style;
		if (settings.bFullScreen)
		{
			style = WS_POPUP;
		}
		else
		{
			style = WS_OVERLAPPEDWINDOW;
		}

		RECT rc = { 0, 0, settings.width, settings.height };
		::AdjustWindowRect(&rc, style, false);
        //创建窗口
#ifdef COMPILER_GCC
		m_HWND = ::CreateWindowA(m_sName.c_str(), m_sName.c_str(),
			style, settings.iLeft, settings.iTop,
			rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInst, NULL);
#else
		m_HWND = ::CreateWindowW(m_wName.c_str(), m_wName.c_str(),
			style, settings.iLeft, settings.iTop,
			rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInst, NULL);
#endif
		//窗口参数
		::GetClientRect(m_HWND, &rc);
		m_iLeft = settings.iLeft;
		m_iTop = settings.iTop;
		m_iWidth = rc.right - rc.left;
		m_iHeight = rc.bottom - rc.top;

		::SetWindowLongPtrW(m_HWND, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		::ShowWindow(m_HWND, SW_SHOWNORMAL);
		::UpdateWindow(m_HWND);
	}

	WIndow::~Window()
	{
		if(m_HWND != NULL)
		{
			::DestroyWindow(m_HWND);
			m_HWND=NULL;
		}
	}

	void Window::Recreate()
	{
		HINSTANCE hInst = ::GetModuleHandle(NULL);

		uint32_t style = static_cast<uint32_t>(::GetWindowLongPtrW(wnd_, GWL_STYLE));
		RECT rc = { 0, 0, width_, height_ };
		::AdjustWindowRect(&rc, style, false);

		::DestroyWindow(wnd_);

#ifdef KLAYGE_COMPILER_GCC
		wnd_ = ::CreateWindowA(name_.c_str(), name_.c_str(),
			style, left_, top_,
			rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInst, NULL);
#else
		wnd_ = ::CreateWindowW(wname_.c_str(), wname_.c_str(),
			style, left_, top_,
			rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInst, NULL);
#endif

		::GetClientRect(wnd_, &rc);
		width_ = rc.right - rc.left;
		height_ = rc.bottom - rc.top;

#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4244)
#endif
		::SetWindowLongPtrW(wnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
#ifdef KLAYGE_COMPILER_MSVC
#pragma warning(pop)
#endif

		::ShowWindow(wnd_, SW_SHOWNORMAL);
		::UpdateWindow(wnd_);
	}
#endif
}
