#pragma once
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Components\Translation\Content\TranslateResultView.h"
#include "Presentation\Components\Translation\TranslationViewModel.h"

class TranslateResultComponent : public Component<TranslateResultView>
{
private:
    ModelHolder<TranslationViewModel*>* modelHolder;

    void ExpandCategory(int categoryIndex) const;

public:
    TranslateResultComponent(ServiceRegistry* serviceRegistry, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder);
};
