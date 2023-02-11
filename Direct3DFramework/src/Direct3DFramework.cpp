// Direct3DFramework.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "Direct3DFramework.h"

#include "Window.h"


int CALLBACK wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Window window = Window(800, 300, L"DirectX Framework");
    Window window2 = Window(200, 640, L"DirectX Framework");

    MSG msg{};
    BOOL result{};

    while (result != GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (result == -1)
    {
        return -1;
    }

    return msg.wParam;
}
