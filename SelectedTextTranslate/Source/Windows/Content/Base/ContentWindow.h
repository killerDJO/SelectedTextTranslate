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
    double kX;
    double kY;

    COLORREF colorGray;
    COLORREF colorBlack;

    HFONT fontNormal, fontHeader, fontItalic, fontSmall;
    HBRUSH grayBrush;

    AppModel* appModel;

    void ComputeParameters() override;
    void InitializeFonts() override;
    void InitializeBrushes() override;

public:
    ContentWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
    virtual ~ContentWindow();

    virtual POINT RenderResult();
};