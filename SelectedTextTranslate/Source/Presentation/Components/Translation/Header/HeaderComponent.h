#pragma once
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Components\Translation\Header\HeaderView.h"
#include "BusinessLogic\Translation\TextPlayer.h"
#include "Presentation\Components\Translation\TranslationViewModel.h"

class HeaderComponent : public Component<HeaderView>
{
private:
    TextPlayer* textPlayer;
    ModelHolder<TranslationViewModel*>* modelHolder;

    void PlayText() const;

public:
    HeaderComponent(ServiceRegistry* serviceRegistry, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder);

    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
};
