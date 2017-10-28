#include "Presentation\Framework\NativeWindowHolder.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

NativeWindowHolder::NativeWindowHolder()
{
    Instance = GetModuleHandle(nullptr);
    BaseWindowProcedure = DefWindowProc;

    Handle = nullptr;
    ClassName = nullptr;
}

void NativeWindowHolder::Initialize()
{
    WNDCLASSEX windowClass = { 0 };

    if(ClassName == nullptr)
    {
        throw SelectedTextTranslateFatalException(L"Window's class name should be provided.");
    }

    if (!GetClassInfoEx(Instance, ClassName, &windowClass))
    {
        windowClass.hInstance = Instance;
        windowClass.lpszClassName = ClassName;
        windowClass.lpfnWndProc = WindowProcedureWrapper;
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

        SpecifyWindowClass(&windowClass);

        AssertCriticalWinApiResult(RegisterClassEx(&windowClass));
    }

    Rect rectangle = GetWindowRectangle();
    Handle = CreateWindowEx(
        GetExtendedWindowStyles(),
        ClassName,
        nullptr,
        GetWindowStyle(),
        rectangle.GetX(),
        rectangle.GetY(),
        rectangle.GetWidth(),
        rectangle.GetHeight(),
        GetWindowParent(),
        nullptr,
        Instance,
        nullptr);
    AssertCriticalWinApiResult(Handle);
    SetWindowLongPtr(Handle, GWLP_USERDATA, (LONG_PTR)this);
}

void NativeWindowHolder::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
}

DWORD NativeWindowHolder::GetWindowStyle() const
{
    return 0;
}

HWND NativeWindowHolder::GetWindowParent() const
{
    return nullptr;
}

Rect NativeWindowHolder::GetWindowRectangle() const
{
    return Rect(0, 0, 0, 0);
}

DWORD NativeWindowHolder::GetExtendedWindowStyles() const
{
    return 0;
}

HWND NativeWindowHolder::GetHandle() const
{
    return Handle;
}

LRESULT NativeWindowHolder::WindowProcedureWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    NativeWindowHolder* windowHolder = (NativeWindowHolder*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (windowHolder == nullptr)
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return windowHolder->ExecuteWindowProcedure(message, wParam, lParam);
}

LRESULT NativeWindowHolder::CallBaseWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) const
{
    return CallWindowProc(BaseWindowProcedure, Handle, message, wParam, lParam);
}

LRESULT NativeWindowHolder::ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    return WindowProcedure(message, wParam, lParam);
}

LRESULT NativeWindowHolder::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    return CallBaseWindowProcedure(message, wParam, lParam);
}

NativeWindowHolder::~NativeWindowHolder()
{
    if(Handle != nullptr)
    {
        AssertCriticalWinApiResult(DestroyWindow(Handle));
    }
}