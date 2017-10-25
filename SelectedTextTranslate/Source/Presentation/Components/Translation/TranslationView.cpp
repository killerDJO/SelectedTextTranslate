#include "Presentation\Components\Translation\TranslationView.h"

TranslationView::TranslationView(CommonContext* context, View* parentView, ModelHolder<TranslateResult>* modelHolder)
    : ComponentView(context, parentView, modelHolder)
{
    headerHeight = ScaleProvider->Scale(50);
    separatorHeight = ScaleProvider->Scale(1);

    headerComponent = nullptr;
    translateResultComponent = nullptr;

    ViewName = L"TranslationWindow";
}

void TranslationView::Initialize()
{
    ComponentView::Initialize();

    ::LayoutDescriptor headerWindowDescriptor = LayoutDescriptor::CreateLayoutDescriptor(
        Point(0, 0),
        Size(0, headerHeight),
        OverflowModes::Stretch,
        OverflowModes::Fixed);
    headerComponent = new HeaderComponent(Context, this, CurrentModelHolder);
    headerComponent->SetLayout(headerWindowDescriptor);
    headerComponent->OnTranslateSuggestion.Subscribe(&OnTranslateSuggestion);
    headerComponent->OnForceTranslation.Subscribe(&OnForceTranslation);
    headerComponent->Initialize();

    ::LayoutDescriptor translateResultWindowDescriptor = LayoutDescriptor::CreateLayoutDescriptor(
        Point(0, headerComponent->GetBoundingRect().GetBottom() + separatorHeight),
        Size(0, 0),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
    translateResultComponent = new TranslateResultComponent(Context, this, CurrentModelHolder);
    translateResultComponent->SetLayout(translateResultWindowDescriptor);
    translateResultComponent->Initialize();
}

Size TranslationView::RenderContent(Renderer* renderer, TranslateResult model)
{
    headerComponent->Render();

    Size contentSize;

    if (model.IsEmptyResult())
    {
        translateResultComponent->MakeHidden();

        Size backgroundSize = Size(
            max(ParentView->GetSize().GetWidth(), headerComponent->GetBoundingRect().GetWidth()),
            ParentView->GetSize().GetHeight() - headerHeight);
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

    RenderSeparator(renderer, max(GetSize().GetWidth(), contentSize.GetWidth()));

    return contentSize;
}

void TranslationView::Resize()
{
    Size parentSize = ParentView->GetSize();
    LayoutDescriptor.SetSize(parentSize);

    ViewStateDescriptor.EnsureSize(parentSize);

    Size bufferingDeviceContextSize = DeviceContextBuffer->GetSize();
    bufferingDeviceContextSize = Size(
        max(parentSize.GetWidth(), bufferingDeviceContextSize.GetWidth()),
        max(parentSize.GetHeight(), bufferingDeviceContextSize.GetHeight()));
    DeviceContextBuffer->Resize(bufferingDeviceContextSize);

    ApplyViewState(true);

    Renderer* renderer = RenderingContext->GetRenderer();

    if(GetModel().IsEmptyResult())
    {
        renderer->DrawRect(Rect(0, headerHeight, ViewStateDescriptor.GetSize().GetWidth(), ViewStateDescriptor.GetSize().GetHeight() - headerHeight), LightGrayBrush);
    }

    RenderSeparator(renderer, max(ViewStateDescriptor.GetContentSize().GetWidth(), ViewStateDescriptor.GetSize().GetWidth()));

    renderer->Render(DeviceContextBuffer);
    RenderingContext->ReleaseRenderer(renderer);

    Draw();
}

void TranslationView::RenderSeparator(Renderer* renderer, int width) const
{
    renderer->DrawRect(Rect(0, headerHeight, width, separatorHeight), LightGrayBrush);
}