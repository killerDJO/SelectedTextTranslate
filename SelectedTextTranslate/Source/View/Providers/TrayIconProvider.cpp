#include "View\Providers\TrayIconProvider.h"
#include "Helpers\ExceptionHelper.h"

TrayIconProvider::TrayIconProvider(AppController* appController)
{
    this->appController = appController;

    CreateMenu();
    WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");
    AssertWinApiResult(WM_TASKBARCREATED);
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

void TrayIconProvider::CreateTrayIcon(HWND windowHandle, HINSTANCE instance)
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

void TrayIconProvider::ShowErrorMessage(wstring message)
{
    wcscpy_s(notifyIconData.szInfoTitle, L"An error occurred");
    wcscpy_s(notifyIconData.szInfo, message.c_str());
    notifyIconData.dwInfoFlags = NIIF_ERROR;

    AssertCriticalWinApiResult(Shell_NotifyIcon(NIM_MODIFY, &notifyIconData));
}

void TrayIconProvider::ProcessTrayIconMessages(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_TRAYICON)
    {
        if (lParam == WM_LBUTTONUP)
        {
            appController->TranslateSelectedText();
        }

        if (lParam == WM_RBUTTONUP)
        {
            POINT curPoint;
            AssertWinApiResult(GetCursorPos(&curPoint));
            SetForegroundWindow(windowHandle);
            UINT clicked = TrackPopupMenu(menu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, windowHandle, nullptr);
            if (clicked == MenuExitItemId)
            {
                appController->Exit();
            }
            if (clicked == MenuTranslateItemId)
            {
                appController->TranslateSelectedText();
            }
            if (clicked == MenuDictionaryItemId)
            {
                appController->ShowDictionary();
            }
        }
    }

    if (message == WM_TASKBARCREATED)
    {
        AssertWinApiResult(Shell_NotifyIcon(NIM_ADD, &notifyIconData));
    }
}

TrayIconProvider::~TrayIconProvider()
{
    DestroyTrayIcon();
}
