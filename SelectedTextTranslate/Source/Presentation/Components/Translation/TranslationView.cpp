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
            max(State->GetWindowSize().GetWidth(), headerComponent->GetBoundingRect().GetWidth()),
            State->GetWindowSize().GetHeight() - headerHeight);
        renderer->DrawRect(Rect(Point(0, headerHeight), backgroundSize), LightGrayBrush);

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

    Size clientSize = ParentView->GetAvailableClientSize();
    RenderSeparator(renderer, max(clientSize.GetWidth(), contentSize.GetWidth()));

    contentSize = Size(
        max(contentSize.GetWidth(), clientSize.GetWidth() - PaddingX),
        contentSize.GetHeight());

    return contentSize;
}

void TranslationView::RenderSeparator(Renderer* renderer, int width) const
{
    renderer->DrawRect(Rect(0, headerHeight, width, separatorHeight), LightGrayBrush);
}
