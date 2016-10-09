#pragma once
#include "Windows\Base\Window.h"
#include "Model\AppModel.h"
#include "Windows\Content\Dictionary\DictionaryWindow.h"
#include "Windows\Content\Translation\TranslationWindow.h"

#define ID_TRAY_APP_ICON    5000
#define WM_TRAYICON ( WM_USER + 1 )
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM          3000
#define ID_TRAY_TRANSLATE_CONTEXT_MENU_ITEM     3002
#define ID_TRAY_DICTIONARY_CONTEXT_MENU_ITEM    3005

class MainWindow : public Window
{
private:
    static UINT WM_TASKBARCREATED;

    HMENU menu;
    NOTIFYICONDATA notifyIconData;

    AppModel* appModel;

    TranslationWindow* translationWindow;
    DictionaryWindow* dictionaryWindow;

    void InitNotifyIconData();
    void CreateViews();

    void Scale(double scaleFactorAjustment);
    void Resize() override;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    Size RenderContent(Renderer* renderer) override;

public:
    MainWindow(WindowContext* context, WindowDescriptor descriptor, AppModel* appModel);
    ~MainWindow();

    void Initialize() override;

    void Minimize();
    void Maximize();
};