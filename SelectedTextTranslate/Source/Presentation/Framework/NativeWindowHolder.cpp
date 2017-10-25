#include "Presentation\Framework\NativeWindowHolder.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

NativeWindowHolder::NativeWindowHolder(HINSTANCE instance)
{
    this->Instance = instance;
    this->BaseWindowProcedure = DefWindowProc;

    this->Handle = nullptr;
    this->ClassName = nullptr;
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
}

void NativeWindowHolder::SpecifyWindowClass(WNDCLASSEX* windowClass)
{
}

HWND NativeWindowHolder::GetHandle() const
{
    return Handle;
}

LRESULT NativeWindowHolder::WindowProcedureWrapper(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    NativeWindowHolder* instance = (NativeWindowHolder*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (instance == nullptr)
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return instance->ExecuteWindowProcedure(message, wParam, lParam);
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
