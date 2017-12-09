#include "Presentation\TrayIcon.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

TrayIcon::TrayIcon(ServiceRegistry* registry)
    : ErrorHandler(registry->Get<Logger>())
{
    logger = registry->Get<Logger>();
    hotkeysRegistry = registry->Get<HotkeysRegistry>();
    messageBus = registry->Get<MessageBus>();

    menu = nullptr;
    WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");
    AssertCriticalWinApiResult(WM_TASKBARCREATED);
    isSuspended = false;

    OnEnable.Subscribe(bind(&TrayIcon::SetEnabledState, this));
    OnSuspend.Subscribe(bind(&TrayIcon::SetSuspendedState, this));

    InitializeMenuActionsToSubscribeableMap();
    CreateNativeWindow();
    RegisterHotkeys();
    CreateMenu();
    CreateTrayIcon();
}

void TrayIcon::CreateNativeWindow()
{
    window = new NativeWindowHolder(this, logger);
    window
        ->SetClassName(L"STT_TRAY")
        ->AddStyles(WS_POPUP)
        ->SetMessageHandler(WM_TRAYICON, bind(&TrayIcon::ProcessTrayIconMessage, this, _1, _2))
        ->SetMessageHandler(WM_TASKBARCREATED, bind(&TrayIcon::ProcessTaskBarCreatedMessage, this, _1, _2))
        ->SetProxyMessageHandler(WM_HOTKEY, bind(&TrayIcon::ProcessHotkeyMessage, this, _1, _2, _3));
}

void TrayIcon::InitializeMenuActionsToSubscribeableMap()
{
    this->menuActionsToSubscribeableMap = map<int, Subscribable<>*>();

    menuActionsToSubscribeableMap[MenuExitItemId] = &messageBus->OnExit;
    menuActionsToSubscribeableMap[MenuTranslateItemId] = &messageBus->OnTranslateSelectedText;
    menuActionsToSubscribeableMap[MenuDictionaryItemId] = &messageBus->OnShowDictionary;
    menuActionsToSubscribeableMap[MenuSettingsItemId] = &messageBus->OnShowSettings;
    menuActionsToSubscribeableMap[MenuSuspendItemId] = &OnSuspend;
    menuActionsToSubscribeableMap[MenuEnableItemId] = &OnEnable;
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
    notifyIconData.hWnd = window->GetHandle();
    notifyIconData.uID = TrayIconId;
    notifyIconData.uVersion = NOTIFYICON_VERSION_4;
    notifyIconData.uFlags = NIF_ICON | NIF_TIP | NIF_INFO | NIF_MESSAGE;
    notifyIconData.uCallbackMessage = WM_TRAYICON;
    notifyIconData.hIcon = LoadIcon(window->GetInstance(), MAKEINTRESOURCE(IDI_APP_ICON));
    wcscpy_s(notifyIconData.szTip, L"Selected text translate..");

    AssertCriticalWinApiResult(Shell_NotifyIcon(NIM_ADD, &notifyIconData));
}

void TrayIcon::SetTrayIconImage(DWORD imageResource)
{
    notifyIconData.hIcon = LoadIcon(window->GetInstance(), MAKEINTRESOURCE(imageResource));
    AssertCriticalWinApiResult(Shell_NotifyIcon(NIM_MODIFY, &notifyIconData));
}

void TrayIcon::RegisterHotkeys()
{
    hotkeysRegistry->RegisterTranslateHotkey(window->GetHandle(), [this]() -> void
    {
        return messageBus->OnTranslateSelectedText();
    });
    hotkeysRegistry->RegisterPlayTextHotkey(window->GetHandle(), [this]() -> void
    {
        return messageBus->OnPlaySelectedText();
    });
}

void TrayIcon::ShowError(wstring message)
{
    wstring truncatedMessage = message.substr(0, min(255, message.length()));

    wcscpy_s(notifyIconData.szInfoTitle, L"An error occurred");
    wcscpy_s(notifyIconData.szInfo, truncatedMessage.c_str());
    notifyIconData.dwInfoFlags = NIIF_ERROR;

    AssertCriticalWinApiResult(Shell_NotifyIcon(NIM_MODIFY, &notifyIconData));

    OnErrorShow();
}

void TrayIcon::SetSuspendedState()
{
    isSuspended = true;
    messageBus->OnSuspendHotkeys();
    DestroyTrayIconMenu();
    CreateMenu();
    SetTrayIconImage(IDI_APP_ICON_DISABLED);
}

void TrayIcon::SetEnabledState()
{
    isSuspended = false;
    messageBus->OnEnableHotkeys();
    DestroyTrayIconMenu();
    CreateMenu();
    SetTrayIconImage(IDI_APP_ICON);
}

LRESULT TrayIcon::ProcessTrayIconMessage(WPARAM wParam, LPARAM lParam)
{
    if (lParam == WM_LBUTTONUP)
    {
        messageBus->OnTranslateSelectedText();
    }

    if (lParam == WM_RBUTTONUP)
    {
        POINT curPoint;
        AssertWinApiResult(GetCursorPos(&curPoint));
        SetForegroundWindow(window->GetHandle());
        UINT clicked = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, window->GetHandle(), nullptr);

        if (clicked != 0)
        {
            menuActionsToSubscribeableMap[clicked]->Notify();
        }
    }

    return 0;
}

LRESULT TrayIcon::ProcessTaskBarCreatedMessage(WPARAM wParam, LPARAM lParam)
{
    AssertWinApiResult(Shell_NotifyIcon(NIM_ADD, &notifyIconData));
    return 0;
}

LRESULT TrayIcon::ProcessHotkeyMessage(WPARAM wParam, LPARAM lParam, function<LRESULT()> baseProcedure)
{
    hotkeysRegistry->ProcessHotkey(wParam);
    return baseProcedure();
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