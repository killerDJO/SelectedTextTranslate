#pragma once
#include "BusinessLogic\Translation\Dto\TranslateResult.h"
#include "Presentation\Framework\ModelHolder.h"
#include "Presentation\Components\Translation\Header\HeaderView.h"
#include "BusinessLogic\Translation\TextPlayer.h"

class HeaderComponent : public Component<HeaderView>
{
private:
    TextPlayer* textPlayer;
    ModelHolder<TranslateResult>* modelHolder;

    void PlayText() const;

public:
    HeaderComponent(CommonContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder);

    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
};