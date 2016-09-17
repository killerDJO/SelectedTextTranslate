#pragma once
#include "Entities\TranslateResult.h"
#include "Windows\Content\Base\ContentWindow.h"
#include "Windows\Buttons\HoverIconButtonWindow.h"
#include "Windows\Buttons\HoverTextButtonWindow.h"

class HeaderWindow : public ContentWindow
{
private:
    HFONT fontSmallUnderscored;

    void PlayText();
    void PrintInputCorrectionWarning(const wchar_t* originalInput, int curY, POINT originLineBottomRight, Renderer* renderer);

protected:
    SIZE RenderDC(Renderer* renderer) override;

public:
    HeaderWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel);
    ~HeaderWindow();

    void Initialize() override;
};