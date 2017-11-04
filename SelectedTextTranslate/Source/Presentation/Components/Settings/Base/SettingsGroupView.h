#pragma once
#include "Presentation\Framework\Rendering\Dto\RenderDescriptor.h"
#include "Presentation\Components\Settings\Base\SettingsGroupHeaderControl.h"
#include "Presentation\Framework\Views\ChildComponentView.h"
#include "Presentation\Components\Settings\Base\SettingsGroupViewModel.h"

template<typename TSettings>
class SettingsGroupView : public ChildComponentView<SettingsGroupViewModel<TSettings>*>
{
private:
    int borderWidth;
    SettingsGroupHeaderControl* headerControl;

protected:
    void RenderContent(Renderer* renderer, SettingsGroupViewModel<TSettings>* model) override;
    virtual void RenderSettingsContent(RenderDescriptor renderDescriptor, TSettings* settings) = 0;

public:
    SettingsGroupView(ViewContext* context, View* parentView, ModelHolder<SettingsGroupViewModel<TSettings>*>* modelHolder, IComponent* component);

    void UpdateHeader();

    Subscribable<> OnSettingsToggled;
    Subscribable<> OnSettingsChanged;
};

template <typename TSettings>
SettingsGroupView<TSettings>::SettingsGroupView(ViewContext* context, View* parentView, ModelHolder<SettingsGroupViewModel<TSettings>*>* modelHolder, IComponent* component)
    : ChildComponentView<SettingsGroupViewModel<TSettings>*>(context, parentView, modelHolder, component)
{
    this->PaddingX = this->PaddingY = this->ScaleProvider->Scale(5);
    this->borderWidth = this->ScaleProvider->Scale(1);
    this->ClassName = L"STT_SETTINGS_GROUP";
    this->headerControl = nullptr;
}

template <typename TSettings>
void SettingsGroupView<TSettings>::UpdateHeader()
{
    if (headerControl != nullptr)
    {
        headerControl->SetContentState(this->GetModel()->GetContentState());
        headerControl->Render();
    }
}

template <typename TSettings>
void SettingsGroupView<TSettings>::RenderContent(Renderer* renderer, SettingsGroupViewModel<TSettings>* model)
{
    this->DestroyChildViews();

    headerControl = new SettingsGroupHeaderControl(this->Context, this);
    headerControl->OnSettingsToggled.Subscribe(&OnSettingsToggled);

    headerControl
        ->SetTitle(model->GetName())
        ->SetContentState(model->GetContentState())
        ->SetVisibilityState(model->GetVisibilityState())
        ->SetDimensions(Point(0, 0), this->State->GetWindowSize().GetWidth())
        ->InitializeAndRender();

    if (model->GetVisibilityState() == SettingsGroupVisibilityState::Expanded)
    {
        RenderSettingsContent(RenderDescriptor(renderer, Point(this->PaddingX * 2, headerControl->GetBoundingRect().GetBottom())), model->GetSettings());
        Rect contentBorderRect = Rect(Point(0, 0), Size(this->State->GetWindowSize().GetWidth(), renderer->GetSize().GetHeight()));
        renderer->DrawBorderedRect(contentBorderRect, nullptr, borderWidth, Colors::Gray);
    }
}
