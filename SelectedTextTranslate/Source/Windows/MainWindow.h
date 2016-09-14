#pragma once
#include "Windows\Base\Window.h"
#include "Windows\Base\renderer.h"
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

    int scrollCharX;
    int scrollCharY;

    static UINT WM_TASKBARCREATED;
    
    HMENU menu;
    NOTIFYICONDATA notifyIconData;

    HeaderWindow* headerWindow;
    TranslateResultWindow* translateResultWindow;
    DictionaryWindow* dictionaryWindow;
    AppModel* appModel;
    Renderer* renderer;

    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;

    void ComputeWindowDimensions(RECT workarea);
    void InitNotifyIconData();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    void InitializeScrollbars(int contentWidth, int contentHeight);
    void InitializeScrollbar(int windowDimension, int contentDimension, int scrollChar, int nBar);
    void ProcessVerticalScroll(WPARAM wParam, LPARAM lParam);
    void ProcessHorizontalScroll(WPARAM wParam, LPARAM lParam);
    void ProcessScroll(WPARAM wParam, LPARAM lParam, int scrollChar, int nBar);
    void SetScrollPosition(SCROLLINFO scrollInfo, int nBar, int scrollOffset, int scrollChar);
    SCROLLINFO GetScrollBarInfo(int nBar);

public:
    MainWindow(HINSTANCE hInstance, AppModel* appModel, Renderer* renderer);
    ~MainWindow();

    void Initialize() override;

    void Minimize();
    void Maximize();

    void ShowTranslateResultView(bool preserveScroll = false);
    void ShowDictionaryView();
};