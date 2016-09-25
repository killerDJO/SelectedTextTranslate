#pragma once
#include "Windows\Content\Base\ContentWindow.h"

class HeaderWindow : public ContentWindow
{
private:
    HFONT fontSmallUnderscored;

    void PlayText() const;
    void PrintInputCorrectionWarning(const wchar_t* originalInput, int curY, Point originLineBottomRight, Renderer* renderer);

protected:
    Size RenderDC(Renderer* renderer) override;

public:
    HeaderWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel);
    ~HeaderWindow();

    void Initialize() override;
};