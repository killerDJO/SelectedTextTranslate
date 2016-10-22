#include "View\Providers\TrayIconProvider.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Utilities\StringUtilities.h"

TrayIconProvider::TrayIconProvider(Logger* logger, HotkeyProvider* hotkeyProvider, HINSTANCE instance)
    : NativeWindowHolder(instance), ErrorHandler(logger)
{
    this->className = L"STT_TRAY";
    this->windowHandle = nullptr;
    this->menu = nullptr;
    this->WM_TASKBARCREATED = 0;

    this->logger = logger;
    this->hotkeyProvider = hotkeyProvider;
}

void TrayIconProvider::Initialize()
{
    NativeWindowHolder::Initialize();
    windowHandle = CreateWindow(className, nullptr, WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, instance, this);
    AssertCriticalWinApiResult(windowHandle);

    WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");
    AssertWinApiResult(WM_TASKBARCREATED);

    hotkeyProvider->RegisterTranslateHotkey(windowHandle, [this]() -> void
    {
        return OnTranslateSelectedText.Notify();
    });
    hotkeyProvider->RegisterPlayTextHotkey(windowHandle, [this]() -> void
    {
        return OnPlaySelectedText.Notify();
    });

    CreateMenu();
    CreateTrayIcon();
}

void TrayIconProvider::CreateMenu()
{
    menu = CreatePopupMenu();
    AssertCriticalWinApiResult(menu);

    AssertCriticalWinApiResult(AppendMenu(menu, MF_STRING, MenuTranslateItemId, TEXT("Translate from clipboard")));
    AssertCriticalWinApiResult(AppendMenu(menu, MF_STRING, MenuDictionaryItemId, TEXT("Dictionary")));
    AssertCriticalWinApiResult(AppendMenu(menu, MF_SEPARATOR, NULL, nullptr));
    AssertCriticalWinApiResult(AppendMenu(menu, MF_STRING, MenuExitItemId, TEXT("Exit")));
}

void TrayIconProvider::CreateTrayIcon()
{
    memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));

    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    notifyIconData.hWnd = windowHandle;
    notifyIconData.uID = TrayIconId;
    notifyIconData.uVersion = NOTIFYICON_VERSION_4;
    notifyIconData.uFlags = NIF_ICON | NIF_TIP | NIF_INFO | NIF_MESSAGE;
    notifyIconData.uCallbackMessage = WM_TRAYICON;
    notifyIconData.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_APP_ICON));
    wcscpy_s(notifyIconData.szTip, L"Selected text translate..");

    AssertCriticalWinApiResult(Shell_NotifyIcon(NIM_ADD, &notifyIconData));
}

void TrayIconProvider::DestroyTrayIcon()
{
    AssertCriticalWinApiResult(Shell_NotifyIcon(NIM_DELETE, &notifyIconData));
}

void TrayIconProvider::ShowError(wstring message)
{
    wstring truncatedMessage = message.substr(0, min(255, message.length()));

    wcscpy_s(notifyIconData.szInfoTitle, L"An error occurred");
    wcscpy_s(notifyIconData.szInfo, truncatedMessage.c_str());
    notifyIconData.dwInfoFlags = NIIF_ERROR;

    AssertCriticalWinApiResult(Shell_NotifyIcon(NIM_MODIFY, &notifyIconData));

    OnErrorShow.Notify();
}

LRESULT TrayIconProvider::ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    try
    {
        ExceptionHelper::SetupStructuredExceptionsTranslation();
        return WindowProcedure(message, wParam, lParam);
    }
    catch (const SelectedTextTranslateException& error)
    {
        ExceptionHelper::HandleNonFatalException(logger, this, L"Error occurred.", error);
    }
    catch (...)
    {
        HandleFatalException();
    }

    return -1;
}

LRESULT TrayIconProvider::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_TRAYICON)
    {
        if (lParam == WM_LBUTTONUP)
        {
            OnTranslateSelectedText.Notify();
        }

        if (lParam == WM_RBUTTONUP)
        {
            POINT curPoint;
            AssertWinApiResult(GetCursorPos(&curPoint));
            SetForegroundWindow(windowHandle);
            UINT clicked = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, windowHandle, nullptr);
            if (clicked == MenuExitItemId)
            {
                OnExit.Notify();
            }
            if (clicked == MenuTranslateItemId)
            {
                OnTranslateSelectedText.Notify();
            }
            if (clicked == MenuDictionaryItemId)
            {
                OnShowDictionary.Notify();
            }
        }

        return 0;
    }

    if (message == WM_TASKBARCREATED)
    {
        AssertWinApiResult(Shell_NotifyIcon(NIM_ADD, &notifyIconData));
        return 0;
    }

    if (message == WM_HOTKEY)
    {
        hotkeyProvider->ProcessHotkey(wParam);
    }

    return NativeWindowHolder::WindowProcedure(message, wParam, lParam);
}

TrayIconProvider::~TrayIconProvider()
{
    DestroyTrayIcon();
}
