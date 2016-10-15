#pragma once
#include "Controller\AppController.h"

class TrayIconProvider
{
    const UINT WM_TRAYICON = WM_USER + 1;

    const int TrayIconId = 5000;

    const int MenuExitItemId = 3000;
    const int MenuTranslateItemId = 3002;
    const int MenuDictionaryItemId = 3005;

    UINT WM_TASKBARCREATED;
    HMENU menu;

    AppController* appController;

    void CreateMenu();

public:
    TrayIconProvider(AppController* appController);
    ~TrayIconProvider();

    NOTIFYICONDATA CreateTrayIcon(HINSTANCE instance, HWND windowHandle) const;
    void DestroyTrayIcon(NOTIFYICONDATA notifyIconData) const;

    void ProcessTrayIconMessages(NOTIFYICONDATA notifyIconData, HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) const;
};