#pragma once
#include "View\Framework\Windows\ContentWindow.h"
#include "Services\Dictionary\Dto\DictionaryRecord.h"
#include "Utilities\Subscribeable.h"
#include "View\Framework\ModelHolder.h"

class DictionaryWindow : public ContentWindow, public ModelHolder<vector<DictionaryRecord>>
{
protected:
    Size RenderContent(Renderer* renderer) override;

public:
    DictionaryWindow(WindowContext* context, Window* parentWindow);
    ~DictionaryWindow() override;

    Subscribeable<int> OnShowTranslation;
};
