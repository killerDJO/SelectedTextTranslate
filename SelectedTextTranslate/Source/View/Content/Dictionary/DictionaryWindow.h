#pragma once
#include "Utilities\Subscribeable.h"
#include "Services\Dictionary\Dto\DictionaryRecord.h"
#include "View\Framework\View\Views\ContentView.h"
#include "View\Framework\ModelHolder.h"

class DictionaryWindow : public ContentView, public ModelHolder<vector<DictionaryRecord>>
{
private:
    int iconSize;

protected:
    Size RenderContent(Renderer* renderer) override;

public:
    DictionaryWindow(ViewContext* context, View* parentView);

    Subscribeable<int> OnShowTranslation;
};
