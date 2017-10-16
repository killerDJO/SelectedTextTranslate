#pragma once
#include "Services\Translation\Dto\TranslateResult.h"
#include "View\Framework\ModelHolder.h"
#include "View\Content\Translation\Header\HeaderView.h"
#include "Services\Translation\TextPlayer.h"

class HeaderComponent : public Component<HeaderView>
{
private:
    TextPlayer* textPlayer;
    ModelHolder<TranslateResult>* modelHolder;

    void PlayText() const;

public:
    HeaderComponent(ViewContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder);

    Subscribeable<> OnForceTranslation;
    Subscribeable<> OnTranslateSuggestion;
};