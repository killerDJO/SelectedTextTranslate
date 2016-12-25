#pragma once
#include "Utilities\Subscribeable.h"
#include "Services\Dictionary\Dto\DictionaryRecord.h"
#include "View\Framework\Windows\ContentWindow.h"
#include "View\Framework\ModelHolder.h"

class DictionaryWindow : public ContentWindow, public ModelHolder<vector<DictionaryRecord>>
{
private:
    int iconSize;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    DictionaryWindow(WindowContext* context, Window* parentWindow);

    Subscribeable<int> OnShowTranslation;
};
