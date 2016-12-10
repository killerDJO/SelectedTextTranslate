#pragma once
#include "View\Framework\Windows\NativeWindowHolder.h"
#include "View\Providers\HotkeyProvider.h"
#include "Infrastructure\Logging\Logger.h"
#include "Infrastructure\ErrorHandling\ErrorHandler.h"

class TrayIconProvider : public NativeWindowHolder, public ErrorHandler
{
    const UINT WM_TRAYICON = WM_USER + 1;

    const int TrayIconId = 5000;

    const int MenuExitItemId = 3000;
    const int MenuTranslateItemId = 3002;
    const int MenuDictionaryItemId = 3005;
    const int MenuSettingsItemId = 3006;
    const int MenuSuspendItemId = 3007;
    const int MenuEnableItemId = 3008;

    map<int, Subscribeable<>*> menuActionsToSubscribeableMap;

    UINT WM_TASKBARCREATED;
    HMENU menu;
    NOTIFYICONDATA notifyIconData;
    bool isSuspended;

    Logger* logger;
    HotkeyProvider* hotkeyProvider;

    void InitializeMenuActionsToSubscribeableMap();

    void CreateMenu();
    void DestroyTrayIconMenu() const;

    void CreateTrayIcon();
    void DestroyTrayIcon();
    void SetTrayIconImage(DWORD imageResource);

    void RegisterHotkeys();
    void UnregisterHotkeys() const;

    LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    TrayIconProvider(Logger* logger, HotkeyProvider* hotkeyProvider, HINSTANCE instance);
    ~TrayIconProvider() override;

    void Initialize() override;

    void ShowError(wstring message) override;

    void SetSuspendedState();
    void SetEnabledState();

    Subscribeable<> OnExit;
    Subscribeable<> OnPlaySelectedText;
    Subscribeable<> OnTranslateSelectedText;
    Subscribeable<> OnShowDictionary;
    Subscribeable<> OnShowSettings;

    Subscribeable<> OnSuspend;
    Subscribeable<> OnEnable;
};