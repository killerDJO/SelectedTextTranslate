#pragma once
#include "Windows\Base\Window.h"
#include "Model\AppModel.h"
#include "Windows\Content\Dictionary\DictionaryWindow.h"
#include "Windows\Content\Translation\TranslationWindow.h"
#include "Windows\Providers\HotkeyProvider.h"
#include "Windows\Providers\TrayIconProvider.h"

class MainWindow : public Window
{
private:
    AppModel* appModel;
    HotkeyProvider* hotkeyProvider;
    TrayIconProvider* trayIconProvider;

    TranslationWindow* translationWindow;
    DictionaryWindow* dictionaryWindow;
    NOTIFYICONDATA notifyIconData;

    void CreateViews();

    void Scale(double scaleFactorAjustment);
    void Resize() override;
    Window* GetCurrentView() const;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    Size RenderContent(Renderer* renderer) override;

public:
    MainWindow(WindowContext* context, WindowDescriptor descriptor, AppModel* appModel, HotkeyProvider* hotkeyProvider, TrayIconProvider* trayIconProvider);
    ~MainWindow();

    void Initialize() override;

    void Minimize();
    void Maximize();
};