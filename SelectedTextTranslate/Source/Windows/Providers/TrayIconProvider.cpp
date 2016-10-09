#include "Windows\Providers\TrayIconProvider.h"

TrayIconProvider::TrayIconProvider(AppModel* appModel)
{
    this->appModel = appModel;

    CreateMenu();
    WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");
}

void TrayIconProvider::CreateMenu()
{
    menu = CreatePopupMenu();
    AppendMenu(menu, MF_STRING, MenuTranslateItemId, TEXT("Translate from clipboard"));
    AppendMenu(menu, MF_STRING, MenuDictionaryItemId, TEXT("Dictionary"));
    AppendMenu(menu, MF_SEPARATOR, NULL, nullptr);
    AppendMenu(menu, MF_STRING, MenuExitItemId, TEXT("Exit"));
}

NOTIFYICONDATA TrayIconProvider::CreateTrayIcon(HINSTANCE instance, HWND windowHandle) const
{
    NOTIFYICONDATA notifyIconData;
    memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));

    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);

    notifyIconData.hWnd = windowHandle;
    notifyIconData.uID = TrayIconId;
    notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    notifyIconData.uCallbackMessage = WM_TRAYICON;
    notifyIconData.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_APP_ICON));
    wcscpy_s(notifyIconData.szTip, TEXT("Selected text translate.."));

    Shell_NotifyIcon(NIM_ADD, &notifyIconData);

    return notifyIconData;
}

void TrayIconProvider::DestroyTrayIcon(NOTIFYICONDATA notifyIconData) const
{
    Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
}

void TrayIconProvider::ProcessTrayIconMessages(NOTIFYICONDATA notifyIconData, HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) const
{
    if(message == WM_TRAYICON)
    {
        if (lParam == WM_LBUTTONUP)
        {
            appModel->TranslateSelectedText();
        }

        if (lParam == WM_RBUTTONUP)
        {
            POINT curPoint;
            GetCursorPos(&curPoint);
            SetForegroundWindow(windowHandle);
            UINT clicked = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, windowHandle, nullptr);
            if (clicked == MenuExitItemId)
            {
                appModel->Exit();
            }
            if (clicked == MenuTranslateItemId)
            {
                appModel->TranslateSelectedText();
            }
            if (clicked == MenuDictionaryItemId)
            {
                appModel->ShowDictionary();
            }
        }
    }

    if (message == WM_TASKBARCREATED)
    {
        Shell_NotifyIcon(NIM_ADD, &notifyIconData);
    }
}

TrayIconProvider::~TrayIconProvider()
{
}
