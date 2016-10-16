#pragma once
#include "Controllers\AppController.h"

class TrayIconProvider
{
    const UINT WM_TRAYICON = WM_USER + 1;

    const int TrayIconId = 5000;

    const int MenuExitItemId = 3000;
    const int MenuTranslateItemId = 3002;
    const int MenuDictionaryItemId = 3005;

    UINT WM_TASKBARCREATED;
    HMENU menu;
    NOTIFYICONDATA notifyIconData;

    AppController* appController;

    void CreateMenu();

public:
    TrayIconProvider(AppController* appController);
    ~TrayIconProvider();

    void CreateTrayIcon(HWND windowHandle, HINSTANCE instance);
    void DestroyTrayIcon();

    void ShowErrorMessage(wstring message);

    void ProcessTrayIconMessages(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
};