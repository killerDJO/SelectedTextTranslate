#pragma once
#include "Windows\Content\Base\ContentWindow.h"
#include "Windows\Content\Translation\HeaderWindow.h"
#include "Windows\Content\Translation\TranslateResultWindow.h"

class TranslationWindow : public ContentWindow
{
private:
    const int headerHeight = 50;
    HBRUSH separatorBrush;

    HeaderWindow* headerWindow;
    TranslateResultWindow* translateResultWindow;

    void RenderSeparator(Renderer* renderer, int width) const;

protected:
    Size RenderContent(Renderer* renderer) override;
    void Resize() override;

public:
    TranslationWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppModel* appModel);
    ~TranslationWindow() override;

    void Initialize() override;
};