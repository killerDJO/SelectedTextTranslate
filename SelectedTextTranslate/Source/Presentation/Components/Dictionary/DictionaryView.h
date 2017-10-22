#pragma once
#include "Utilities\Subscribeable.h"
#include "BusinessLogic\Dictionary\Dto\DictionaryRecord.h"
#include "Presentation\Framework\Views\ComponentView.h"

class DictionaryView : public ComponentView<vector<DictionaryRecord>>
{
private:
    int iconSize;

protected:
    Size RenderContent(Renderer* renderer, vector<DictionaryRecord> model) override;

public:
    DictionaryView(CommonContext* context, View* parentView, ModelHolder<vector<DictionaryRecord>>* modelHolder);

    Subscribeable<int> OnShowTranslation;
};