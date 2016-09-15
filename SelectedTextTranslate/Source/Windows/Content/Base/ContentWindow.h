#pragma once
#include "Windows\Base\ChildWindow.h"
#include "AppModel.h"

class ContentWindow : public ChildWindow
{
protected:
    const UINT fontHeight = 20;

    UINT paddingX;
    UINT paddingY;
    UINT lineHeight;

    HFONT fontNormal, fontHeader, fontItalic, fontSmall;
    HBRUSH grayBrush;

    DWORD dcWidth;
    DWORD dcHeight;

    AppModel* appModel;

    void InitializeInMemoryDC() override;
    POINT RenderDC() override;

public:
    ContentWindow(Renderer* renderer, AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
    virtual ~ContentWindow();

    void Initialize() override;
    virtual POINT RenderResult();
};