#pragma once
#include "View\Framework\Windows\NativeWindowHolder.h"
#include "Infrastructure\Logging\Logger.h"
#include "Infrastructure\ErrorHandling\ErrorHandler.h"

class TrayIconProvider : public NativeWindowHolder, public ErrorHandler
{
    const UINT WM_TRAYICON = WM_USER + 1;

    const int TrayIconId = 5000;

    const int MenuExitItemId = 3000;
    const int MenuTranslateItemId = 3002;
    const int MenuDictionaryItemId = 3005;

    UINT WM_TASKBARCREATED;
    HMENU menu;
    NOTIFYICONDATA notifyIconData;

    function<void()> exitCallback;
    function<void()> translateSelectedTextCallback;
    function<void()> showDictionaryCallback;

    Logger* logger;

    void CreateMenu();
    void DestroyTrayIcon();
    void CreateTrayIcon();

    LRESULT ExecuteWindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;
    LRESULT WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam) override;

public:
    TrayIconProvider(Logger* logger, HINSTANCE instance);
    ~TrayIconProvider() override;

    void Initialize() override;

    void ShowError(wstring message) override;

    void SetExitCallback(function<void()> exitCallback);
    void SetTranslateSelectedTextCallback(function<void()> translateSelectedTextCallback);
    void SetShowDictionaryCallback(function<void()> showDictionaryCallback);
};