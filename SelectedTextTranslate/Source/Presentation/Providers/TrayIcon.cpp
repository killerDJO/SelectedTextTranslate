#include "Presentation\Providers\TrayIcon.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

TrayIcon::TrayIcon(ServiceRegistry* registry)
    : NativeWindowHolder(), ErrorHandler(logger)
{
    logger = registry->Get<Logger>();
    hotkeysRegistry = registry->Get<HotkeysRegistry>();
    messageBus = registry->Get<MessageBus>();

    ClassName = L"STT_TRAY";
    menu = nullptr;
    WM_TASKBARCREATED = 0;
    isSuspended = false;

    OnEnable.Subscribe(bind(&TrayIcon::SetEnabledState, this));
    OnSuspend.Subscribe(bind(&TrayIcon::SetSuspendedState, this));

    InitializeMenuActionsToSubscribeableMap();
}

void TrayIcon::InitializeMenuActionsToSubscribeableMap()
{
    this->menuActionsToSubscribeableMap = map<int, Subscribeable<>*>();

    menuActionsToSubscribeableMap[MenuExitItemId] = &messageBus->OnExit;
    menuActionsToSubscribeableMap[MenuTranslateItemId] = &messageBus->OnTranslateSelectedText;
    menuActionsToSubscribeableMap[MenuDictionaryItemId] = &messageBus->OnShowDictionary;
    menuActionsToSubscribeableMap[MenuSettingsItemId] = &messageBus->OnShowSettings;
    menuActionsToSubscribeableMap[MenuSuspendItemId] = &OnSuspend;
    menuActionsToSubscribeableMap[MenuEnableItemId] = &OnEnable;
}

void TrayIcon::Initialize()
{
    NativeWindowHolder::Initialize();

    WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");
    AssertWinApiResult(WM_TASKBARCREATED);

    RegisterHotkeys();
    CreateMenu();
    CreateTrayIcon();
}

DWORD TrayIcon::GetWindowStyle() const
{
    return NativeWindowHolder::GetWindowStyle() | WS_POPUP;
}

void TrayIcon::CreateMenu()
{
    menu = CreatePopupMenu();
    AssertCriticalWinApiResult(menu);

    AssertCriticalWinApiResult(AppendMenu(menu, MF_STRING, MenuTranslateItemId, TEXT("Translate from clipboard")));
    AssertCriticalWinApiResult(AppendMenu(menu, MF_STRING, MenuDictionaryItemId, TEXT("Dictionary")));
    AssertCriticalWinApiResult(AppendMenu(menu, MF_STRING, MenuSettingsItemId, TEXT("Settings")));
    AssertCriticalWinApiResult(AppendMenu(menu, MF_SEPARATOR, NULL, nullptr));

    if(isSuspended)
    {
        AssertCriticalWinApiResult(AppendMenu(menu, MF_STRING, MenuEnableItemId, TEXT("Enable")));
    }
    else
    {
        AssertCriticalWinApiResult(AppendMenu(menu, MF_STRING, MenuSuspendItemId, TEXT("Suspend")));
    }

    AssertCriticalWinApiResult(AppendMenu(menu, MF_SEPARATOR, NULL, nullptr));
    AssertCriticalWinApiResult(AppendMenu(menu, MF_STRING, MenuExitItemId, TEXT("Exit")));
}

void TrayIcon::CreateTrayIcon()
{
    memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));

    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    notifyIconData.hWnd = Handle;
    notifyIconData.uID = TrayIconId;
    notifyIconData.uVersion = NOTIFYICON_VERSION_4;
    notifyIconData.uFlags = NIF_ICON | NIF_TIP | NIF_INFO | NIF_MESSAGE;
    notifyIconData.uCallbackMessage = WM_TRAYICON;
    notifyIconData.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDI_APP_ICON));
    wcscpy_s(notifyIconData.szTip, L"Selected text translate..");

    AssertCriticalWinApiResult(Shell_NotifyIcon(NIM_ADD, &notifyIconData));
}

void TrayIcon::SetTrayIconImage(DWORD imageResource)
{
    notifyIconData.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(imageResource));
    AssertCriticalWinApiResult(Shell_NotifyIcon(NIM_MODIFY, &notifyIconData));
}

void TrayIcon::RegisterHotkeys()
{
    hotkeysRegistry->RegisterTranslateHotkey(Handle, [this]() -> void
    {
        return messageBus->OnTranslateSelectedText.Notify();
    });
    hotkeysRegistry->RegisterPlayTextHotkey(Handle, [this]() -> void
    {
        return messageBus->OnPlaySelectedText.Notify();
    });
}

void TrayIcon::ShowError(wstring message)
{
    wstring truncatedMessage = message.substr(0, min(255, message.length()));

    wcscpy_s(notifyIconData.szInfoTitle, L"An error occurred");
    wcscpy_s(notifyIconData.szInfo, truncatedMessage.c_str());
    notifyIconData.dwInfoFlags = NIIF_ERROR;

    AssertCriticalWinApiResult(Shell_NotifyIcon(NIM_MODIFY, &notifyIconData));

    OnErrorShow.Notify();
}

void TrayIcon::SetSuspendedState()
{
    isSuspended = true;
    messageBus->OnSuspendHotkeys.Notify();
    DestroyTrayIconMenu();
    CreateMenu();
    SetTrayIconImage(IDI_APP_ICON_DISABLED);
}

void TrayIcon::SetEnabledState()
{
    isSuspended = false;
    messageBus->OnEnableHotkeys.Notify();
    DestroyTrayIconMenu();
    CreateMenu();
    SetTrayIconImage(IDI_APP_ICON);
}

LRESULT TrayIcon::ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT TrayIcon::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_TRAYICON)
    {
        if (lParam == WM_LBUTTONUP)
        {
            messageBus->OnTranslateSelectedText.Notify();
        }

        if (lParam == WM_RBUTTONUP)
        {
            POINT curPoint;
            AssertWinApiResult(GetCursorPos(&curPoint));
            SetForegroundWindow(Handle);
            UINT clicked = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, Handle, nullptr);

            if(clicked != 0)
            {
                menuActionsToSubscribeableMap[clicked]->Notify();
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
        hotkeysRegistry->ProcessHotkey(wParam);
    }

    return NativeWindowHolder::WindowProcedure(message, wParam, lParam);
}

void TrayIcon::DestroyTrayIcon()
{
    Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
}

void TrayIcon::DestroyTrayIconMenu() const
{
    AssertCriticalWinApiResult(DestroyMenu(menu));
}

TrayIcon::~TrayIcon()
{
    DestroyTrayIcon();
    DestroyTrayIconMenu();
}