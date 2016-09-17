#pragma once
#include "Windows\Framework\RenderingContext.h"
#include "Windows\Framework\ScrollProvider.h"
#include "Windows\Framework\Dto\WindowDescriptor.h"

class Window
{
protected:

    HWND hWindow;
    HINSTANCE hInstance;
    wchar_t* className;

    WindowDescriptor descriptor;
    int currentWidth;
    int currentHeight;

    RenderingContext* renderingContext;
    ScrollProvider* scrollProvider;

    virtual void SpecifyWindowClass(WNDCLASSEX* windowClass) = 0;
    virtual SIZE RenderContent() = 0;

    DWORD GetScrollStyle();

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    Window(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor);
    virtual ~Window();

    HWND GetHandle();
    HINSTANCE GetInstance();
    int GetWidth();
    int GetHeight();

    virtual void Initialize();
    void Show();
    void Hide();

    void Render(bool preserveVerticalScroll = false);
};