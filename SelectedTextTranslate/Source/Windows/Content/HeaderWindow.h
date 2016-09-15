#pragma once
#include "Entities\TranslateResult.h"
#include "Windows\Content\Base\ContentWindow.h"
#include "Windows\Buttons\HoverIconButtonWindow.h"
#include "Windows\Buttons\HoverTextButtonWindow.h"

class HeaderWindow : public ContentWindow
{
protected:
    POINT RenderDC() override;

private:
    HFONT fontSmallUnderscored;

    void PlayText();
    void HeaderWindow::PrintInputCorrectionWarning(const wchar_t* originalInput, int curY, POINT originLineBottomRight, POINT* bottomRight);

public:
    HeaderWindow(Renderer* renderer, AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD height);
    ~HeaderWindow();

    void Initialize() override;
};