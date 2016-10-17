#include "View\Framework\Windows\WindowHolder.h"
#include "ErrorHandling\ExceptionHelper.h"

WindowHolder::WindowHolder(HINSTANCE instance)
{
    this->instance = instance;

    this->windowHandle = nullptr;
    this->className = nullptr;
}

void WindowHolder::Initialize()
{
    WNDCLASSEX windowClass = { 0 };

    if (!GetClassInfoEx(instance, className, &windowClass))
    {
        windowClass.hInstance = instance;
        windowClass.lpszClassName = className;
        windowClass.lpfnWndProc = WindowProcedureWrapper;
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

        SpecifyWindowClass(&windowClass);

        AssertCriticalWinApiResult(RegisterClassEx(&windowClass));
    }
}

void WindowHolder::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
}

HWND WindowHolder::GetHandle() const
{
    return windowHandle;
}

LRESULT WindowHolder::WindowProcedureWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WindowHolder* instance;

    if (message == WM_CREATE)
    {
        // Store pointer to the current window class in the GWLP_USERDATA. 'this' must be passed as lpParam in CreateWindow call.
        CREATESTRUCT* createstruct = (CREATESTRUCT*)lParam;
        instance = (WindowHolder*)createstruct->lpCreateParams;
        instance->windowHandle = hWnd;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)createstruct->lpCreateParams);
    }
    else
    {
        instance = (WindowHolder*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (instance == nullptr)
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return instance->ExecuteWindowProcedure(message, wParam, lParam);
}

LRESULT WindowHolder::ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    return WindowProcedure(message, wParam, lParam);
}

LRESULT WindowHolder::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(windowHandle, message, wParam, lParam);
}

WindowHolder::~WindowHolder()
{
    AssertCriticalWinApiResult(DestroyWindow(windowHandle));
}
