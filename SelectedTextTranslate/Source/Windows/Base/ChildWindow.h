#pragma once
#include "Windows\Base\Window.h"

class ChildWindow : public Window
{
private:
    void InitializeInMemoryDC();
    void DestroyChildWindows();
    void Draw();
    void ClearHDC(HDC hdc);

protected:
    HWND parentWindow;
    HDC inMemoryHDC;
    vector<ChildWindow*> childWindows;

    void SpecifyWindowClass(WNDCLASSEX* windowClass) override;

    virtual void ComputeParameters();
    virtual void InitializeFonts();
    virtual void InitializeBrushes();
    virtual POINT RenderDC();

    HDC CreateInMemoryHDC(DWORD hdcWidth, DWORD hdcHeight);
    void ResizeHDC(HDC &hdc, DWORD width, DWORD height);

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