#include "Presentation\Framework\NativeWindowHolder.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"

NativeWindowHolder::NativeWindowHolder(ErrorHandler* errorHandler, Logger* logger)
{
    this->logger = logger;
    this->errorHandler = errorHandler;

    instance = GetModuleHandle(nullptr);
    baseWindowProcedure = DefWindowProc;

    handle = nullptr;

    styles = 0;
    extendedStyles = 0;
    isSubClass = false;
    parent = nullptr;
}

NativeWindowHolder* NativeWindowHolder::Initialize()
{
    WNDCLASSEX windowClass = { 0 };

    if(className.empty())
    {
        throw SelectedTextTranslateFatalException(L"Window's class name should be provided.");
    }

    if (!GetClassInfoEx(instance, className.c_str(), &windowClass))
    {
        windowClass.hInstance = instance;
        windowClass.lpszClassName = className.c_str();
        windowClass.lpfnWndProc = WindowProcedureWrapper;
        windowClass.cbSize = sizeof(WNDCLASSEX);
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        windowClass.cbWndExtra = 2 * sizeof(LONG_PTR); //TODO: get rid

        for(auto classSpecifier : classSpecifiers)
        {
            if (classSpecifier)
            {
                classSpecifier(&windowClass);
            }
        }

        AssertCriticalWinApiResult(RegisterClassEx(&windowClass));
    }

    handle = CreateWindowEx(
        extendedStyles,
        className.c_str(),
        nullptr,
        styles,
        boundingRect.GetX(),
        boundingRect.GetY(),
        boundingRect.GetWidth(),
        boundingRect.GetHeight(),
        parent,
        nullptr,
        instance,
        nullptr);
    AssertCriticalWinApiResult(handle);
    SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)this);

    if (isSubClass)
    {
        baseWindowProcedure = (WNDPROC)SetWindowLongPtr(handle, GWLP_WNDPROC, (LONG_PTR)WindowProcedureWrapper);
        AssertCriticalWinApiResult(baseWindowProcedure);
    }

    return this;
}

NativeWindowHolder* NativeWindowHolder::SetClassName(wstring className)
{
    this->className = className;
    return this;
}

NativeWindowHolder* NativeWindowHolder::AddStyles(DWORD styles)
{
    this->styles |= styles;
    return this;
}

NativeWindowHolder* NativeWindowHolder::AddExtendedStyles(DWORD extendedStyles)
{
    this->extendedStyles |= extendedStyles;
    return this;
}

NativeWindowHolder* NativeWindowHolder::SetParent(HWND parent)
{
    this->parent = parent;
    return this;
}

NativeWindowHolder* NativeWindowHolder::SetBoundingRect(Rect boundingRect)
{
    this->boundingRect = boundingRect;
    return this;
}

NativeWindowHolder* NativeWindowHolder::EnableSubclassing()
{
    isSubClass = true;
    return this;
}

NativeWindowHolder* NativeWindowHolder::AddClassSpecifier(function<void(WNDCLASSEX*)> classSpecifier)
{
    classSpecifiers.push_back(classSpecifier);
    return this;
}

NativeWindowHolder* NativeWindowHolder::SetMessageHandler(UINT message, function<LRESULT(WPARAM, LPARAM)> messageHandler)
{
    messageHandlers[message] = messageHandler;
    return this;
}

NativeWindowHolder* NativeWindowHolder::SetMessageHandler(UINT message, function<void(WPARAM, LPARAM)> messageHandler, LRESULT result)
{
    return SetMessageHandler(message, [messageHandler, result](WPARAM wParam, LPARAM lParam) -> LRESULT {
        messageHandler(wParam, lParam);
        return result;
    });
}

NativeWindowHolder* NativeWindowHolder::SetMessageHandler(UINT message, LRESULT result)
{
    return SetMessageHandler(message, [result](WPARAM wParam, LPARAM lParam) -> LRESULT {
        return result;
    });
}

NativeWindowHolder* NativeWindowHolder::SetProxyMessageHandler(UINT message, function<LRESULT(WPARAM, LPARAM, function<LRESULT()>)> messageHandler)
{
    return SetMessageHandler(message, [messageHandler, this, message](WPARAM wParam, LPARAM lParam) -> LRESULT {
        return messageHandler(wParam, lParam, [this, message, wParam, lParam]() { return CallBaseWindowProcedure(message, wParam, lParam); });
    });
}

NativeWindowHolder* NativeWindowHolder::SetMessageHandler(function<void(UINT, WPARAM, LPARAM)> messageHandler)
{
    commonMessageHandlers.push_back(messageHandler);
    return this;
}

HWND NativeWindowHolder::GetHandle() const
{
    return handle;
}

HINSTANCE NativeWindowHolder::GetInstance() const
{
    return instance;
}

wstring NativeWindowHolder::GetClass() const
{
    return className;
}

void NativeWindowHolder::Show()
{
    ShowWindow(handle, SW_SHOW);
}

void NativeWindowHolder::Hide()
{
    ShowWindow(handle, SW_HIDE);
}

void NativeWindowHolder::Move(Rect boundingRect)
{
    AssertCriticalWinApiResult(MoveWindow(
        handle,
        boundingRect.GetX(),
        boundingRect.GetY(),
        boundingRect.GetWidth(),
        boundingRect.GetHeight(),
        FALSE));
}

void NativeWindowHolder::BringToFront()
{
    SwitchToThisWindow(handle, TRUE);
}

void NativeWindowHolder::Focus()
{
    AssertCriticalWinApiResult(SetFocus(handle));
}

void NativeWindowHolder::ApplCursor(LPCWSTR name)
{
    SetCursor(LoadCursor(nullptr, name));
}

Rect NativeWindowHolder::GetBoundingRect()
{
    RECT windowRect;
    AssertCriticalWinApiResult(GetWindowRect(handle, &windowRect));
    return Rect(
        windowRect.left,
        windowRect.top,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top);
}

void NativeWindowHolder::DrawNonClientArea()
{
    SendMessage(handle, WM_NCPAINT, NULL, NULL);
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
    return CallWindowProc(baseWindowProcedure, handle, message, wParam, lParam);
}

LRESULT NativeWindowHolder::ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    try
    {
        ExceptionHelper::SetupStructuredExceptionsTranslation();
        return WindowProcedure(message, wParam, lParam);
    }
    catch (const SelectedTextTranslateException& error)
    {
        ExceptionHelper::HandleNonFatalException(logger, errorHandler, L"Error occurred.", error);
    }
    catch (...)
    {
        errorHandler->HandleFatalException();
    }

    return -1;
}

LRESULT NativeWindowHolder::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    for(auto messageHandler : commonMessageHandlers)
    {
        if(messageHandler)
        {
            messageHandler(message, wParam, lParam);
        }
    }

    if (messageHandlers.count(message) && messageHandlers[message])
    {
        return messageHandlers[message](wParam, lParam);
    }

    return CallBaseWindowProcedure(message, wParam, lParam);
}

NativeWindowHolder::~NativeWindowHolder()
{
    if(handle != nullptr)
    {
        AssertCriticalWinApiResult(DestroyWindow(handle));
    }
}