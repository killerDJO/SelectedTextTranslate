#include "Presentation\Components\Translation\TranslationView.h"

TranslationView::TranslationView(ViewContext* context, View* parentView, ModelHolder<TranslationViewModel*>* modelHolder, IComponent* component)
    : ChildComponentView(context, parentView, modelHolder, component)
{
    headerHeight = ScaleProvider->Scale(50);
    separatorHeight = ScaleProvider->Scale(1);

    headerComponent = nullptr;
    translateResultComponent = nullptr;

    Name = L"TranslationWindow";
}

void TranslationView::Initialize()
{
    ChildComponentView::Initialize();

    LayoutDescriptor headerWindowDescriptor = LayoutDescriptor::CreateLayoutDescriptor(
        Point(0, 0),
        Size(0, headerHeight),
        OverflowModes::Stretch,
        OverflowModes::Fixed);
    headerComponent = new HeaderComponent(Context->GetServiceRegistry(), this, CurrentModelHolder);
    headerComponent->SetLayout(headerWindowDescriptor);
    headerComponent->OnTranslateSuggestion.Subscribe(&OnTranslateSuggestion);
    headerComponent->OnForceTranslation.Subscribe(&OnForceTranslation);
    headerComponent->Initialize();

    LayoutDescriptor translateResultWindowDescriptor = LayoutDescriptor::CreateLayoutDescriptor(
        Point(0, headerComponent->GetBoundingRect().GetBottom() + separatorHeight),
        Size(0, 0),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
    translateResultComponent = new TranslateResultComponent(Context->GetServiceRegistry(), this, CurrentModelHolder);
    translateResultComponent->SetLayout(translateResultWindowDescriptor);
    translateResultComponent->Initialize();
}

Size TranslationView::RenderContent(Renderer* renderer, TranslationViewModel* model)
{
    headerComponent->Render();

    Size contentSize;
    if (model->GetTranslateResult().IsEmptyResult())
    {
        translateResultComponent->MakeHidden();

        Size backgroundSize = Size(
            max(ParentView->GetBoundingRect().GetWidth(), headerComponent->GetBoundingRect().GetWidth()),
            ParentView->GetBoundingRect().GetHeight() - headerHeight);
        renderer->DrawRect(Rect(Point(0, headerHeight), backgroundSize), BackgroundBrush);

        contentSize = headerComponent->GetBoundingRect().GetSize();
    }
    else
    {
        translateResultComponent->MakeVisible();
        translateResultComponent->Render();

        contentSize = Size(
            max(headerComponent->GetBoundingRect().GetWidth(), translateResultComponent->GetBoundingRect().GetWidth()),
            headerComponent->GetBoundingRect().GetHeight() + translateResultComponent->GetBoundingRect().GetHeight());
    }

    RenderSeparator(renderer, max(GetBoundingRect().GetWidth(), contentSize.GetWidth()));

    return contentSize;
}

void TranslationView::Resize()
{
    Size parentSize = ParentView->GetBoundingRect().GetSize();
    Layout.SetSize(parentSize);

    State.EnsureSize(parentSize);

    Size bufferingDeviceContextSize = DeviceContextBuffer->GetSize();
    bufferingDeviceContextSize = Size(
        max(parentSize.GetWidth(), bufferingDeviceContextSize.GetWidth()),
        max(parentSize.GetHeight(), bufferingDeviceContextSize.GetHeight()));
    DeviceContextBuffer->Resize(bufferingDeviceContextSize);

    ApplyViewState(true);

    Renderer* renderer = RenderingContext->GetRenderer();

    if(GetModel()->GetTranslateResult().IsEmptyResult())
    {
        renderer->DrawRect(Rect(0, headerHeight, State.GetSize().GetWidth(), State.GetSize().GetHeight() - headerHeight), LightGrayBrush);
    }

    RenderSeparator(renderer, max(State.GetContentSize().GetWidth(), State.GetSize().GetWidth()));

    renderer->Render(DeviceContextBuffer);
    RenderingContext->ReleaseRenderer(renderer);

    Draw();
}

void TranslationView::RenderSeparator(Renderer* renderer, int width) const
{
    renderer->DrawRect(Rect(0, headerHeight, width, separatorHeight), LightGrayBrush);
}
