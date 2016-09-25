#pragma once
#include "Windows\Content\Base\ContentWindow.h"
#include "Windows\Controls\Buttons\HoverIconButtonWindow.h"

class DictionaryWindow : public ContentWindow
{
protected:
    Size RenderDC(Renderer* renderer) override;

private:
    void ShowFullTranslation(int dictionaryIndex) const;

public:
    DictionaryWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel);
    ~DictionaryWindow();
};