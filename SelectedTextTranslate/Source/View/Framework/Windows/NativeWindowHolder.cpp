#include "View\Framework\Windows\NativeWindowHolder.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

NativeWindowHolder::NativeWindowHolder(HINSTANCE instance)
{
    this->instance = instance;

    this->windowHandle = nullptr;
    this->className = nullptr;
}

void NativeWindowHolder::Initialize()
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

void NativeWindowHolder::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
}

HWND NativeWindowHolder::GetHandle() const
{
    return windowHandle;
}

LRESULT NativeWindowHolder::WindowProcedureWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    NativeWindowHolder* instance;

    if (message == WM_CREATE)
    {
        // Store pointer to the current window class in the GWLP_USERDATA. 'this' must be passed as lpParam in CreateWindow call.
        CREATESTRUCT* createstruct = (CREATESTRUCT*)lParam;
        instance = (NativeWindowHolder*)createstruct->lpCreateParams;
        instance->windowHandle = hWnd;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)createstruct->lpCreateParams);
    }
    else
    {
        instance = (NativeWindowHolder*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    }

    if (instance == nullptr)
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return instance->ExecuteWindowProcedure(message, wParam, lParam);
}

LRESULT NativeWindowHolder::ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    return WindowProcedure(message, wParam, lParam);
}

LRESULT NativeWindowHolder::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(windowHandle, message, wParam, lParam);
}

NativeWindowHolder::~NativeWindowHolder()
{
    AssertCriticalWinApiResult(DestroyWindow(windowHandle));
}
