
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <SE3DEngine/Config/Platform.h>
#include <SE3DEngine/AppLayer/RenderConfig.h>
#include <SE3DEngine/Comm/Def.h>

#ifdef COMPILER_MSVS
#pragma warning(push)
#pragma warning(disable: 4100 4512 6011)
#endif
#include <boost/signals2.hpp>
#ifdef COMPILER_MSVS
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
			return x_display_;
		}

		::XVisualInfo* VisualInfo() const
		{
			return vi_;
		}

		::Window XWindow() const
		{
			return x_window_;
		}
#elif defined SE_ANDROID
		::ANativeWindow* AWindow() const
		{
			return a_window_;
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
		//事件
		/*
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
		typedef boost::signals2::signal<void(Window const &, uint32_t, Vector_T<int32_t, 2> const &)> MouseDownEvent;
		typedef boost::signals2::signal<void(Window const &, uint32_t, Vector_T<int32_t, 2> const &)> MouseUpEvent;
		typedef boost::signals2::signal<void(Window const &, uint32_t, Vector_T<int32_t, 2> const &, int32_t)> MouseWheelEvent;
		typedef boost::signals2::signal<void(Window const &, uint32_t, Vector_T<int32_t, 2> const &)> MouseOverEvent;
		typedef boost::signals2::signal<void(Window const &)> CloseEvent;

		ActiveEvent& OnActive()
		{
			return active_event_;
		}
		PaintEvent& OnPaint()
		{
			return paint_event_;
		}
		EnterSizeMoveEvent& OnEnterSizeMove()
		{
			return enter_size_move_event_;
		}
		ExitSizeMoveEvent& OnExitSizeMove()
		{
			return exit_size_move_event_;
		}
		SizeEvent& OnSize()
		{
			return size_event_;
		}
		SetCursorEvent& OnSetCursor()
		{
			return set_cursor_event_;
		}
		CharEvent& OnChar()
		{
			return char_event_;
		}
		KeyDownEvent& OnKeyDown()
		{
			return key_down_event_;
		}
		KeyUpEvent& OnKeyUp()
		{
			return key_up_event_;
		}
		MouseDownEvent& OnMouseDown()
		{
			return mouse_down_event_;
		}
		MouseUpEvent& OnMouseUp()
		{
			return mouse_up_event_;
		}
		MouseWheelEvent& OnMouseWheel()
		{
			return mouse_wheel_event_;
		}
		MouseOverEvent& OnMouseOver()
		{
			return mouse_over_event_;
		}
		CloseEvent& OnClose()
		{
			return close_event_;
		}

	private:
		ActiveEvent active_event_;
		PaintEvent paint_event_;
		EnterSizeMoveEvent enter_size_move_event_;
		ExitSizeMoveEvent exit_size_move_event_;
		SizeEvent size_event_;
		SetCursorEvent set_cursor_event_;
		CharEvent char_event_;
		KeyDownEvent key_down_event_;
		KeyUpEvent key_up_event_;
		MouseDownEvent mouse_down_event_;
		MouseUpEvent mouse_up_event_;
		MouseWheelEvent mouse_wheel_event_;
		MouseOverEvent mouse_over_event_;
		CloseEvent close_event_;
		*/

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
		::Display* x_display_;
		::GLXFBConfig* fbc_;
		::XVisualInfo* vi_;
		::Window x_window_;
		::Atom wm_delete_window_;
#elif defined SE_ANDROID
		::ANativeWindow* a_window_;
#endif
	};
}

#endif