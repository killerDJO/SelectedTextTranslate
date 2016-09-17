#pragma once
#include "Windows\Base\Window.h"
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
    static UINT WM_TASKBARCREATED;

    HMENU menu;
    NOTIFYICONDATA notifyIconData;

    AppModel* appModel;
    RenderingContext* renderingContext;

    HeaderWindow* headerWindow;
    TranslateResultWindow* translateResultWindow;
    DictionaryWindow* dictionaryWindow;

    void InitNotifyIconData();

    SIZE RenderTranslateResultView();
    SIZE RenderDictionaryView();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    SIZE RenderContent() override;

public:
    MainWindow(HINSTANCE hInstance, AppModel* appModel, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor);
    ~MainWindow();

    void Initialize() override;

    void Minimize();
    void Maximize();
};