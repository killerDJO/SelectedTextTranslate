#pragma once
#include "Windows\Content\Base\ContentWindow.h"
#include "Entities\TranslateResult.h"
#include "Windows\Controls\Buttons\HoverTextButtonWindow.h"

class TranslateResultWindow : public ContentWindow
{
private:
    HFONT fontUnderscored;

    void ExpandDictionary(int index) const;
    int CreateExpandButton(TranslateResultDictionary category, int categoryIndex, int showedCount, int curY, Renderer* renderer);

protected:
    Size RenderDC(Renderer* renderer) override;

public:
    TranslateResultWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel);
    ~TranslateResultWindow();

    void Initialize() override;
};