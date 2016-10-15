#pragma once
#include "View\Content\Base\ContentWindow.h"
#include "Services\Translation\Dto\TranslateResult.h"

class TranslateResultWindow : public ContentWindow
{
private:
    HFONT fontUnderscored;

    void ExpandDictionary(int index) const;
    int CreateExpandButton(TranslateResultDictionary category, int categoryIndex, int showedCount, int curY, Renderer* renderer);

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    TranslateResultWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppController* appController);
    ~TranslateResultWindow() override;

    void Initialize() override;
};