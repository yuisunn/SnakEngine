// 用类似 qt 的信号 和插槽的东西实现窗口事件处理

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <SE3DEngine/Config/Platform.h>
#include <SE3DEngine/AppLayer/RenderConfig.h>
#include <SE3DEngine/Comm/Def.h>
#include <SE3DEngine/Comm/VectorT.h>
 #define _CRT_SECURE_NO_DEPRECATE
#ifdef COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4100 4512 6011)
#endif
#include <boost/signals2.hpp>
#ifdef COMPILER_MSVC
#pragma warning(pop)
#endif

#if defined SE_WINDOWS
#include <windows.h>
#elif defined SE_LINUX
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#elif defined SE_ANDROID
#include <android_native_app_glue.h>
#endif

#include <string>

namespace SE
{
	enum MouseButtons
	{
		MB_None = 0,
		MB_Left = 1UL << 0,
		MB_Right = 1UL << 1,
		MB_Middle = 1UL << 2,
		MB_Shift = 1UL << 3,
		MB_Ctrl = 1UL << 4,
		MB_Alt = 1UL << 5
	};

	class DLL_API Window
	{
	public:
		Window(std::string const& cpName, SRenderConfig const& cpSettings);
		~Window();

#if defined SE_WINDOWS
		void Recreate();
#endif

#if defined SE_WINDOWS
		HWND GetHWnd() const
		{
			return m_HWND;
		}
#elif defined SE_LINUX
		::Display* XDisplay() const
		{
			return m_XDisplay;
		}

		::XVisualInfo* VisualInfo() const
		{
			return m_Vi;
		}

		::Window XWindow() const
		{
			return m_XWindow;
		}
#elif defined SE_ANDROID
		::ANativeWindow* AWindow() const
		{
			return m_AWindow;
		}
#endif

		int32 GetLeft() const
		{
			return m_iLeft;
		}
		int32 GetTop() const
		{
			return m_iTop;
		}

		uint32 GetWidth() const
		{
			return m_iWidth;
		}
		uint32 GetHeight() const
		{
			return m_iHeight;
		}
		 
		//型号和插槽
	public:
		typedef boost::signals2::signal<void(Window const &, bool)> ActiveEvent;
		typedef boost::signals2::signal<void(Window const &)> PaintEvent;
		typedef boost::signals2::signal<void(Window const &)> EnterSizeMoveEvent;
		typedef boost::signals2::signal<void(Window const &)> ExitSizeMoveEvent;
		typedef boost::signals2::signal<void(Window const &, bool)> SizeEvent;
		typedef boost::signals2::signal<void(Window const &)> SetCursorEvent;
		typedef boost::signals2::signal<void(Window const &, wchar_t)> CharEvent;
		typedef boost::signals2::signal<void(Window const &, wchar_t)> KeyDownEvent;
		typedef boost::signals2::signal<void(Window const &, wchar_t)> KeyUpEvent;
		typedef boost::signals2::signal<void(Window const &, uint32, Vector_T<int32, 2> const &)> MouseDownEvent;
		typedef boost::signals2::signal<void(Window const &, uint32, Vector_T<int32, 2> const &)> MouseUpEvent;
		typedef boost::signals2::signal<void(Window const &, uint32, Vector_T<int32, 2> const &, int32_t)> MouseWheelEvent;
		typedef boost::signals2::signal<void(Window const &, uint32, Vector_T<int32, 2> const &)> MouseOverEvent;
		typedef boost::signals2::signal<void(Window const &)> CloseEvent;
		//返回 型号和操的函数
		ActiveEvent& OnActive()
		{
			return m_ActiveEvent;
		}
		PaintEvent& OnPaint()
		{
			return m_PaintEvent;
		}
		EnterSizeMoveEvent& OnEnterSizeMove()
		{
			return m_EnterSizeMoveEvent;
		}
		ExitSizeMoveEvent& OnExitSizeMove()
		{
			return m_ExitSizeMoveEvent;
		}
		SizeEvent& OnSize()
		{
			return m_SizeEvent;
		}
		SetCursorEvent& OnSetCursor()
		{
			return m_SetCursorEvent;
		}
		CharEvent& OnChar()
		{
			return m_CharEvent;
		}
		KeyDownEvent& OnKeyDown()
		{
			return m_KeyDownEvent;
		}
		KeyUpEvent& OnKeyUp()
		{
			return m_KeyUpEvent;
		}
		MouseDownEvent& OnMouseDown()
		{
			return m_MouseDownEvent;
		}
		MouseUpEvent& OnMouseUp()
		{
			return m_MouseUpEvent;
		}
		MouseWheelEvent& OnMouseWheel()
		{
			return m_MouseWheelEvent;
		}
		MouseOverEvent& OnMouseOver()
		{
			return m_MouseOverEvent;
		}
		CloseEvent& OnClose()
		{
			return m_CloseEvent;
		}

	private:
		ActiveEvent m_ActiveEvent;
		PaintEvent m_PaintEvent;
		EnterSizeMoveEvent m_EnterSizeMoveEvent;
		ExitSizeMoveEvent m_ExitSizeMoveEvent;
		SizeEvent m_SizeEvent;
		SetCursorEvent m_SetCursorEvent;
		CharEvent m_CharEvent;
		KeyDownEvent m_KeyDownEvent;
		KeyUpEvent m_KeyUpEvent;
		MouseDownEvent m_MouseDownEvent;
		MouseUpEvent m_MouseUpEvent;
		MouseWheelEvent m_MouseWheelEvent;
		MouseOverEvent m_MouseOverEvent;
		CloseEvent m_CloseEvent;
		

#if defined SE_WINDOWS
	private:
		//消息循环
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,
			WPARAM wParam, LPARAM lParam);
		
		LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#elif defined SE_LINUX
	public:
		void MsgProc(XEvent const & event);
#elif defined SE_ANDROID
	public:
		static void MsgProc(android_app* app, int32_t cmd);
#endif

	private:
		int32 m_iLeft;
		int32 m_iTop;
		uint32 m_iWidth;
		uint32 m_iHeight;

#if defined SE_WINDOWS
#ifdef COMPILER_GCC
		std::string m_sName;
#else
		std::wstring m_wName;
#endif

		HWND m_HWND;
#elif defined SE_LINUX
		::Display* m_XDisplay;
		::GLXFBConfig* m_Fbc;
		::XVisualInfo* m_Vi;
		::Window m_XWindow;
		::Atom m_WmDeleteWindow;
#elif defined SE_ANDROID
		::ANativeWindow* m_AWindow;
#endif
	};
}

#endif