#pragma once
#include "Infrastructure\ErrorHandling\ErrorHandler.h"
#include "Infrastructure\Logging\Logger.h"
#include "Presentation\Framework\NativeWindowHolder.h"
#include "Presentation\Providers\HotkeysRegistry.h"
#include "Presentation\MessageBus.h"

class TrayIcon : public NativeWindowHolder, public ErrorHandler
{
private:
    const UINT WM_TRAYICON = WM_USER + 1;

    const int TrayIconId = 5000;

    const int MenuExitItemId = 3000;
    const int MenuTranslateItemId = 3002;
    const int MenuDictionaryItemId = 3005;
    const int MenuSettingsItemId = 3006;
    const int MenuSuspendItemId = 3007;
    const int MenuEnableItemId = 3008;

    map<int, Subscribable<>*> menuActionsToSubscribeableMap;

    UINT WM_TASKBARCREATED;
    HMENU menu;
    NOTIFYICONDATA notifyIconData;
    bool isSuspended;

    Logger* logger;
    HotkeysRegistry* hotkeysRegistry;
    MessageBus* messageBus;

    void InitializeMenuActionsToSubscribeableMap();

    void CreateMenu();
    void DestroyTrayIconMenu() const;

    void CreateTrayIcon();
    void DestroyTrayIcon();
    void SetTrayIconImage(DWORD imageResource);

    void RegisterHotkeys();

    LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

    Subscribable<> OnSuspend;
    Subscribable<> OnEnable;

    void SetSuspendedState();
    void SetEnabledState();

protected:
    DWORD GetWindowStyle() const override;

public:
    TrayIcon(ServiceRegistry* registry);
    ~TrayIcon() override;

    void Initialize() override;

    void ShowError(wstring message) override;
};
