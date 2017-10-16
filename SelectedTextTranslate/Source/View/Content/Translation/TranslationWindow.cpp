#include "View\Content\Translation\TranslationWindow.h"

TranslationWindow::TranslationWindow(ViewContext* context, View* parentWindow)
    : ContentView(context, parentWindow)
{
    this->headerHeight = context->GetScaleProvider()->Scale(50);
    this->separatorHeight = context->GetScaleProvider()->Scale(1);

    this->headerWindow = nullptr;
    this->translateResultWindow = nullptr;
}

void TranslationWindow::Initialize()
{
    ContentView::Initialize();

    WindowDescriptor headerWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, 0),
        Size(0, headerHeight),
        OverflowModes::Stretch,
        OverflowModes::Fixed);
    headerWindow = new HeaderWindow(context, this);
    headerWindow->SetDescriptor(headerWindowDescriptor);
    headerWindow->OnPlayText.Subscribe(&OnPlayText);
    headerWindow->OnTranslateSuggestion.Subscribe(&OnTranslateSuggestion);
    headerWindow->OnForceTranslation.Subscribe(&OnForceTranslation);
    headerWindow->Initialize();

    WindowDescriptor translateResultWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, headerWindow->GetBoundingRect().GetBottom() + separatorHeight),
        Size(0, 0),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
    translateResultWindow = new TranslateResultWindow(context, this);
    translateResultWindow->SetDescriptor(translateResultWindowDescriptor);
    translateResultWindow->OnRequestRender.Subscribe(&OnRequestRender);
    translateResultWindow->Initialize();
}

void TranslationWindow::SetModel(TranslateResult translateResult)
{
    ModelHolder<TranslateResult>::SetModel(translateResult);
    this->headerWindow->SetModel(translateResult);
    this->translateResultWindow->SetModel(translateResult);
}

Size TranslationWindow::RenderContent(Renderer* renderer)
{
    headerWindow->Render();

    Size contentSize;

    if (model.IsEmptyResult())
    {
        translateResultWindow->MakeHidden();

        Size backgroundSize = Size(
            max(parentWindow->GetSize().GetWidth(), headerWindow->GetSize().GetWidth()),
            parentWindow->GetSize().GetHeight() - headerHeight);
        renderer->DrawRect(Rect(Point(0, headerHeight), backgroundSize), backgroundBrush);

        contentSize = headerWindow->GetSize();
    }
    else
    {
        translateResultWindow->MakeVisible();
        translateResultWindow->Render();

        contentSize = Size(
            max(headerWindow->GetSize().GetWidth(), translateResultWindow->GetSize().GetWidth()),
            headerWindow->GetSize().GetHeight() + translateResultWindow->GetSize().GetHeight());
    }

    RenderSeparator(renderer, max(GetSize().GetWidth(), contentSize.GetWidth()));

    return contentSize;
}

void TranslationWindow::Resize()
{
    Size parentSize = parentWindow->GetSize();
    descriptor.SetWindowSize(parentSize);

    nativeStateDescriptor.EnsureSize(parentSize);

    Size bufferingDeviceContextSize = deviceContextBuffer->GetSize();
    bufferingDeviceContextSize = Size(
        max(parentSize.GetWidth(), bufferingDeviceContextSize.GetWidth()),
        max(parentSize.GetHeight(), bufferingDeviceContextSize.GetHeight()));
    deviceContextBuffer->Resize(bufferingDeviceContextSize);

    ApplyNativeState(true);

    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    if(model.IsEmptyResult())
    {
        renderer->DrawRect(Rect(0, headerHeight, nativeStateDescriptor.GetSize().GetWidth(), nativeStateDescriptor.GetSize().GetHeight() - headerHeight), lightGrayBrush);
    }

    RenderSeparator(renderer, max(contentSize.GetWidth(), nativeStateDescriptor.GetSize().GetWidth()));

    renderer->Render(deviceContextBuffer);
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    Draw();
}

void TranslationWindow::RenderSeparator(Renderer* renderer, int width) const
{
    renderer->DrawRect(Rect(0, headerHeight, width, separatorHeight), lightGrayBrush);
}