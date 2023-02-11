#pragma once

#include "framework.h"

class Window
{
private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept; 
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass(WindowClass&&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		WindowClass& operator=(WindowClass&&) = delete;
		static constexpr const char* s_WndClassName{ "Direct3D framework" };
		static WindowClass s_WndClass;
		HINSTANCE m_HInst;
	};

public:
	Window(int width, int height, LPCWSTR name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
	int m_Width{};
	int m_Height{};
	HWND m_hWnd{};
};

