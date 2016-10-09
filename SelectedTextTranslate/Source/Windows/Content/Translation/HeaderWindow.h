#pragma once
#include "Windows\Content\Base\ContentWindow.h"

class HeaderWindow : public ContentWindow
{
private:
    HFONT fontSmallUnderscored;

    void PlayText() const;
    void PrintInputCorrectionWarning(const wchar_t* originalInput, int curY, Point originLineBottomRight, Renderer* renderer);

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    HeaderWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppModel* appModel);
    ~HeaderWindow() override;

    void Initialize() override;
};