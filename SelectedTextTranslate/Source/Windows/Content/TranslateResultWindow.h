#pragma once
#include "Windows\Content\Base\ContentWindow.h"
#include "Entities\TranslateResult.h"
#include "Windows\Buttons\HoverTextButtonWindow.h"

class TranslateResultWindow : public ContentWindow
{
protected:
    POINT RenderDC() override;
    void InitializeFonts() override;

private:
    HFONT fontUnderscored;

    void ExpandDictionary(int index);

public:
    TranslateResultWindow(AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height);
    ~TranslateResultWindow();
};