#pragma once
#include "Windows\Base\Window.h"
#include "Windows\Framework\ScrollProvider.h"
#include "Windows\Content\HeaderWindow.h"
#include "Windows\Content\DictionaryWindow.h"
#include "Windows\Content\TranslateResultWindow.h"

#define ID_TRAY_APP_ICON    5000
#define WM_TRAYICON ( WM_USER + 1 )

#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM          3000
#define ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM     3002
#define ID_TRANSLATE_HOTKEY                     3003
#define ID_PLAYTEXT_HOTKEY                      3004
#define ID_TRAY_DICTIONARY_CONTEXT_MENU_ITEM    3005

class MainWindow : public Window
{
private:
    UINT padding;

    static UINT WM_TASKBARCREATED;

    HMENU menu;
    NOTIFYICONDATA notifyIconData;

    AppModel* appModel;
    Renderer* renderer;
    ScrollProvider* scrollProvider;

    HeaderWindow* headerWindow;
    TranslateResultWindow* translateResultWindow;
    DictionaryWindow* dictionaryWindow;

    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;

    void ComputeWindowDimensions(RECT workarea);
    void InitNotifyIconData();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    MainWindow(HINSTANCE hInstance, AppModel* appModel, Renderer* renderer, ScrollProvider* scrollProvider);
    ~MainWindow();

    void Initialize() override;

    void Minimize();
    void Maximize();

    void ShowTranslateResultView(bool preserveScroll = false);
    void ShowDictionaryView();
};