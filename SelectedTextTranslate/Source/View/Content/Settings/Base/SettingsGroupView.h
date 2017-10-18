#pragma once
#include "View\Framework\Rendering\Dto\RenderDescriptor.h"
#include "View\Content\Settings\Base\SettingsGroupHeaderControl.h"
#include "View\Framework\View\Views\ComponentView.h"
#include "View\Content\Settings\Base\Dto\SettingsGroupState.h"

template<typename TSettings>
class SettingsGroupView : public ComponentView<SettingsGroupState<TSettings>>
{
private:
    int borderWidth;

protected:
    Size RenderContent(Renderer* renderer, SettingsGroupState<TSettings>* model) override;
    virtual void RenderSettingsContent(RenderDescriptor renderDescriptor, TSettings* settings) = 0;

public:
    SettingsGroupView(ViewContext* context, View* parentView, ModelHolder<SettingsGroupState<TSettings>*>* modelHolder);

    Subscribeable<> OnSettingsToggled;
    Subscribeable<> OnSettingsChanged;
};

template <typename TSettings>
SettingsGroupView<TSettings>::SettingsGroupView(ViewContext* context, View* parentView, ModelHolder<SettingsGroupState<TSettings>*>* modelHolder)
    : ComponentView<SettingsGroupState<TSettings>>(context, parentView, modelHolder)
{
    this->paddingX = this->paddingY = this->scaleProvider->Scale(5);
    this->borderWidth = this->scaleProvider->Scale(1);
    this->className = L"STT_SETTINGS_GROUP";
    this->headerWindow = nullptr;
}

template <typename TSettings>
Size SettingsGroupView<TSettings>::RenderContent(Renderer* renderer, SettingsGroupState<TSettings>* model)
{
    this->DestroyChildViews();

    SettingsGroupHeaderControl* headerControl = new SettingsGroupHeaderControl(this->context, this);
    headerControl->SetTitle(model->GetTitle());
    headerControl->SetContentState(model->GetContentState());
    headerControl->SetVisibilityState(model->GetVisibilityState());
    headerControl->SetDimensions(Point(0, 0), this->GetSize().GetWidth());
    headerControl->OnSettingsToggled.Subscribe(&OnSettingsToggled);
    headerControl->InitializeAndRender();

    renderer->UpdateRenderedContentSize(headerControl);

    if (model->GetVisibilityState() == SettingsGroupVisibilityState::Expanded)
    {
        RenderSettingsContent(RenderDescriptor(renderer, Point(this->paddingX * 2, headerControl->GetBoundingRect().GetBottom())), &model->GetSettings());
        Rect contentBorderRect = Rect(Point(0, 0), Size(this->GetSize().GetWidth(), renderer->GetSize().GetHeight()));
        renderer->DrawBorderedRect(contentBorderRect, nullptr, borderWidth, Colors::Gray);
    }

    return renderer->GetSize();
}