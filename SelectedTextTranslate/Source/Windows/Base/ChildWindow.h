#pragma once
#include "Windows\Base\Window.h"

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

    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;

    virtual void InitializeInMemoryDC();
    virtual void InitializeFonts();
    virtual void InitializeBrushes();
    virtual POINT RenderDC();

    void AddChildWindow(ChildWindow* childWindow);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    ChildWindow(Renderer* renderer, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
    virtual ~ChildWindow();

    void Initialize() override;
};