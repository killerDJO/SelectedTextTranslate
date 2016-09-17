#pragma once
#include "Windows\Base\Window.h"
#include "Windows\Framework\Renderer.h"

class ChildWindow : public Window
{
private:
    vector<ChildWindow*> activeChildWindows;
    vector<ChildWindow*> destroyBeforeDrawQueue;

    void Draw();
    void ForceDraw();
    void DrawChildWindows();

    void DestroyChildWindows(vector<ChildWindow*>& childWindows);

protected:
    HWND parentWindow;

    HDC inMemoryDC;
    int dcWidth;
    int dcHeight;

    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;
    SIZE RenderContent() override;

    virtual SIZE RenderDC(Renderer* renderer) = 0;
    void AddChildWindow(ChildWindow* childWindow);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    ChildWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow);
    ~ChildWindow() override;

    void Initialize() override;
};