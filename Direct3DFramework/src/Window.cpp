#include "pch.h"
#include "Window.h"

Window::WindowClass Window::WindowClass::s_WndClass{};

Window::WindowClass::WindowClass() noexcept
	: m_HInst(GetModuleHandle(nullptr))
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = HandleMsgSetup;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetInstance();
    wcex.hIcon = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = (LPCWSTR)GetName();
    wcex.hIconSm = nullptr;

    RegisterClassExW(&wcex);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClassW((LPCWSTR)s_WndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
    return s_WndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return s_WndClass.m_HInst;
}

Window::Window(int width, int height, LPCWSTR name) noexcept
{
    RECT wr{};
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    m_hWnd = CreateWindowW(
        (LPCWSTR)WindowClass::GetName(), (LPCWSTR)name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowClass::GetInstance(), this // We pass a reference to this custom data
    );

    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(m_hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // WM_NCCREATE is called before window creation
    if (msg == WM_NCCREATE)
    {
        // The WM_NNCREATE event specifically holds the creation struct at the lParam
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);

        // Fetch the custom data that we appointed in the Window class
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

        // Set WinAPI-managed user data to hold our window class
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

        // Set the default WndProcedure to our custom function
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

        // Let out member function handle the rest
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // Get our custom window data fromt the WinAPI storage
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    // Forward message to window class handler
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}