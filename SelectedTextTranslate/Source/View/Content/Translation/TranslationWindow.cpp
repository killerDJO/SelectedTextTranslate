#include "View\Content\Translation\TranslationWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

TranslationWindow::TranslationWindow(WindowContext* context, Window* parentWindow)
    : ContentWindow(context, parentWindow)
{
    this->OnPlayText = Subscribeable<>();
    this->OnForceTranslation = Subscribeable<>();
    this->OnTranslateSuggestion = Subscribeable<>();
    this->OnExpandTranslationResult = Subscribeable<int>();

    this->headerWindow = nullptr;
    this->translateResultWindow = nullptr;
}

void TranslationWindow::Initialize()
{
    ContentWindow::Initialize();

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
    AddChildWindow(headerWindow);

    WindowDescriptor translateResultWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, headerHeight + 1),
        Size(0, 0),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
    translateResultWindow = new TranslateResultWindow(context, this);
    translateResultWindow->SetDescriptor(translateResultWindowDescriptor);
    translateResultWindow->OnExpandTranslationResult.Subscribe(&OnExpandTranslationResult);
    AddChildWindow(translateResultWindow);
}

void TranslationWindow::SetModel(TranslateResult translateResult)
{
    ModelHolder<TranslateResult>::SetModel(translateResult);
    this->headerWindow->SetModel(translateResult);
    this->translateResultWindow->SetModel(translateResult);
}

Size TranslationWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);

    headerWindow->Render();

    Size contentSize;

    if (model.IsEmptyResult())
    {
        translateResultWindow->MakeHidden();

        Size backgroundSize = Size(
            max(parentWindow->GetSize().Width, headerWindow->GetSize().Width),
            parentWindow->GetSize().Height - headerHeight);
        renderer->DrawRect(Rect(Point(0, headerHeight), backgroundSize), backgroundBrush);
        contentSize = headerWindow->GetSize(false);
    }
    else
    {
        translateResultWindow->MakeVisible();
        translateResultWindow->Render();

        contentSize.Width = max(headerWindow->GetSize(false).Width, translateResultWindow->GetSize(false).Width);
        contentSize.Height = headerWindow->GetSize(false).Height + translateResultWindow->GetSize(false).Height;
    }

    RenderSeparator(renderer, max(windowSize.Width, contentSize.Width));

    return contentSize;
}

void TranslationWindow::Resize()
{
    Size parentSize = parentWindow->GetSize(false);
    descriptor.SetWindowSize(parentSize);

    windowSize.Width = max(parentSize.Width, windowSize.Width);
    windowSize.Height = max(parentSize.Height, windowSize.Height);

    Size bufferingDeviceContextSize = deviceContextBuffer->GetSize();
    bufferingDeviceContextSize.Width = max(parentSize.Width, bufferingDeviceContextSize.Width);
    bufferingDeviceContextSize.Height = max(parentSize.Height, bufferingDeviceContextSize.Height);
    deviceContextBuffer->Resize(bufferingDeviceContextSize);

    AssertCriticalWinApiResult(MoveWindow(windowHandle, position.X, position.Y, windowSize.Width, windowSize.Height, FALSE));

    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    if(model.IsEmptyResult())
    {
        renderer->DrawRect(Rect(0, headerHeight, windowSize.Width, windowSize.Height - headerHeight), lightGrayBrush);
    }

    RenderSeparator(renderer, max(contentSize.Width, windowSize.Width));

    renderer->Render(deviceContextBuffer);
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    Draw();
}

void TranslationWindow::RenderSeparator(Renderer* renderer, int width) const
{
    int height = 1;
    int scaledWidth = context->GetScaleProvider()->Downscale(width);
    renderer->DrawRect(Rect(0, headerHeight, scaledWidth, height), lightGrayBrush);
}

TranslationWindow::~TranslationWindow()
{
}