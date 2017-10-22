#pragma once
#include "Presentation\Framework\Rendering\Dto\RenderDescriptor.h"
#include "Presentation\Components\Settings\Base\SettingsGroupHeaderControl.h"
#include "Presentation\Framework\Views\ComponentView.h"
#include "Presentation\Components\Settings\Base\SettingsGroupViewModel.h"

template<typename TSettings>
class SettingsGroupView : public ComponentView<SettingsGroupViewModel<TSettings>*>
{
private:
    int borderWidth;
    SettingsGroupHeaderControl* headerControl;

protected:
    Size RenderContent(Renderer* renderer, SettingsGroupViewModel<TSettings>* model) override;
    virtual void RenderSettingsContent(RenderDescriptor renderDescriptor, TSettings* settings) = 0;

public:
    SettingsGroupView(CommonContext* context, View* parentView, ModelHolder<SettingsGroupViewModel<TSettings>*>* modelHolder);

    void UpdateHeader();

    Subscribeable<> OnSettingsToggled;
    Subscribeable<> OnSettingsChanged;
};

template <typename TSettings>
SettingsGroupView<TSettings>::SettingsGroupView(CommonContext* context, View* parentView, ModelHolder<SettingsGroupViewModel<TSettings>*>* modelHolder)
    : ComponentView<SettingsGroupViewModel<TSettings>*>(context, parentView, modelHolder)
{
    this->paddingX = this->paddingY = this->scaleProvider->Scale(5);
    this->borderWidth = this->scaleProvider->Scale(1);
    this->className = L"STT_SETTINGS_GROUP";
    this->headerControl = nullptr;
}

template <typename TSettings>
void SettingsGroupView<TSettings>::UpdateHeader()
{
    if (headerControl != nullptr)
    {
        headerControl->SetContentState(this->modelHolder->GetModel()->GetContentState());
        headerControl->Render();
    }
}

template <typename TSettings>
Size SettingsGroupView<TSettings>::RenderContent(Renderer* renderer, SettingsGroupViewModel<TSettings>* model)
{
    this->DestroyChildViews();

    headerControl = new SettingsGroupHeaderControl(this->context, this);
    headerControl->SetTitle(model->GetName());
    headerControl->SetContentState(model->GetContentState());
    headerControl->SetVisibilityState(model->GetVisibilityState());
    headerControl->SetDimensions(Point(0, 0), this->GetSize().GetWidth());
    headerControl->OnSettingsToggled.Subscribe(&OnSettingsToggled);
    headerControl->InitializeAndRender();

    renderer->UpdateRenderedContentSize(headerControl);

    if (model->GetVisibilityState() == SettingsGroupVisibilityState::Expanded)
    {
        RenderSettingsContent(RenderDescriptor(renderer, Point(this->paddingX * 2, headerControl->GetBoundingRect().GetBottom())), model->GetSettings());
        Rect contentBorderRect = Rect(Point(0, 0), Size(this->GetSize().GetWidth(), renderer->GetSize().GetHeight()));
        renderer->DrawBorderedRect(contentBorderRect, nullptr, borderWidth, Colors::Gray);
    }

    return renderer->GetSize();
}
