#pragma once
#include "Windows\Base\Window.h"

class ChildWindow : public Window
{
private:
    vector<ChildWindow*> activeChildWindows;
    vector<ChildWindow*> childWindowsToDestory;

    void InitializeInMemoryDC();
    void DestroyChildWindows(vector<ChildWindow*>* childWindows);
    void Draw();
    void RedrawChildWindows();
    void ForceRedraw();
    void ClearDC(HDC hdc);

protected:
    HWND parentWindow;
    HDC inMemoryDC;

    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;

    virtual void ComputeParameters();
    virtual void InitializeFonts();
    virtual void InitializeBrushes();

    
    virtual POINT RenderDC();

    HDC CreateInMemoryDC(DWORD hdcWidth, DWORD hdcHeight);
    void ResizeDC(HDC &hdc, DWORD width, DWORD height);
    DWORD CopyDC(HDC source, HDC target);

    void AddChildWindow(ChildWindow* childWindow);

    DWORD AdjustToResolution(double value, double k);
    POINT PrintText(HDC hdc, const wchar_t* text, HFONT font, COLORREF color, int x, int y, PPOINT bottomRight);
    void DrawRect(HDC hdc, RECT rect, HBRUSH brush, PPOINT bottomRight);
    SIZE GetTextSize(HDC hdc, const wchar_t* text, HFONT font);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    ChildWindow(HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
    virtual ~ChildWindow();

    void Initialize() override;
};