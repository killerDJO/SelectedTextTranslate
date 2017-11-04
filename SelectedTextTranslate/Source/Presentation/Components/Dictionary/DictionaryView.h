#pragma once
#include "Utilities\Subscribable\Subscribable.h"
#include "BusinessLogic\Dictionary\Dto\DictionaryRecord.h"
#include "Presentation\Framework\Views\ChildComponentView.h"

class DictionaryView : public ChildComponentView<vector<DictionaryRecord>>
{
private:
    int iconSize;

protected:
    void RenderContent(Renderer* renderer, vector<DictionaryRecord> model) override;

public:
    DictionaryView(ViewContext* context, View* parentView, ModelHolder<vector<DictionaryRecord>>* modelHolder, IComponent* component);

    Subscribable<int> OnShowTranslation;
};