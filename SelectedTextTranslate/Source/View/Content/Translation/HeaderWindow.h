#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "Services\Translation\Dto\TranslateResult.h"
#include "Utilities\Subscribeable.h"

class HeaderWindow : public ContentWindow
{
private:
    HFONT fontSmallUnderscored;

    TranslateResult translateResult;

    void PrintInputCorrectionWarning(wstring originalInput, int curY, Point originLineBottomRight, Renderer* renderer);

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    HeaderWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow);
    ~HeaderWindow() override;

    Subscribeable<> OnPlayText;
    Subscribeable<> OnForceTranslation;

    void Initialize() override;

    void SetModel(TranslateResult translateResult);
};
