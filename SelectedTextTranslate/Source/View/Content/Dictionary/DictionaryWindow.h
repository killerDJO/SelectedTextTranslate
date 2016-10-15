#pragma once
#include "View\Content\Base\ContentWindow.h"

class DictionaryWindow : public ContentWindow
{
protected:
    Size RenderContent(Renderer* renderer) override;

private:
    void ShowFullTranslation(int dictionaryIndex) const;

public:
    DictionaryWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppModel* appModel);
    ~DictionaryWindow() override;
};