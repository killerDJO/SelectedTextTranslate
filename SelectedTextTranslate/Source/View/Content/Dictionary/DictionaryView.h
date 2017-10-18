#pragma once
#include "Utilities\Subscribeable.h"
#include "Services\Dictionary\Dto\DictionaryRecord.h"
#include "View\Framework\View\Views\ComponentView.h"

class DictionaryView : public ComponentView<vector<DictionaryRecord>>
{
private:
    int iconSize;

protected:
    Size RenderContent(Renderer* renderer, vector<DictionaryRecord> model) override;

public:
    DictionaryView(ViewContext* context, View* parentView, ModelHolder<vector<DictionaryRecord>>* modelHolder);

    Subscribeable<int> OnShowTranslation;
};