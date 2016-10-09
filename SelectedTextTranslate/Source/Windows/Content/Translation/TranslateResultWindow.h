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
    Size RenderContent(Renderer* renderer) override;

public:
    TranslateResultWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppModel* appModel);
    ~TranslateResultWindow() override;

    void Initialize() override;
};