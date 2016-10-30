#include "View\Content\Translation\TranslationWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

TranslationWindow::TranslationWindow(WindowContext* context, WindowDescriptor descriptor, wstring name, Window* parentWindow)
    : ContentWindow(context, descriptor, name, parentWindow)
{
    this->separatorBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::LightGray);
    this->disabledBackgroundBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::Disabled);
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
    headerWindow = new HeaderWindow(context, headerWindowDescriptor, L"HeaderWindow", this);
    headerWindow->OnPlayText.Subscribe(&OnPlayText);
    headerWindow->OnTranslateSuggestion.Subscribe(&OnTranslateSuggestion);
    headerWindow->OnForceTranslation.Subscribe(&OnForceTranslation);
    AddChildWindow(headerWindow);

    WindowDescriptor translateResultWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, headerHeight + 1),
        Size(0, 0),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
    translateResultWindow = new TranslateResultWindow(context, translateResultWindowDescriptor, L"TranslateResultWindow", this);
    translateResultWindow->OnExpandTranslationResult.Subscribe(&OnExpandTranslationResult);
    AddChildWindow(translateResultWindow);
}

void TranslationWindow::SetModel(TranslateResult translateResult)
{
    AssertWindowInitialized();

    this->translateResult = translateResult;
    this->headerWindow->SetModel(translateResult);
    this->translateResultWindow->SetModel(translateResult);
}

Size TranslationWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);

    headerWindow->Render();

    Size contentSize;

    if (translateResult.IsEmptyResult())
    {
        translateResultWindow->Hide();

        Size backgroundSize = Size(
            max(parentWindow->GetSize().Width, headerWindow->GetSize().Width),
            parentWindow->GetSize().Height - headerHeight);
        renderer->DrawRect(Rect(Point(0, headerHeight), backgroundSize), disabledBackgroundBrush);

        contentSize = headerWindow->GetSize();
    }
    else
    {
        translateResultWindow->Show();
        translateResultWindow->Render();

        contentSize.Width = max(headerWindow->GetSize().Width, translateResultWindow->GetSize().Width);
        contentSize.Height = headerWindow->GetSize().Height + translateResultWindow->GetSize().Height;
    }

    RenderSeparator(renderer, max(windowSize.Width, contentSize.Width));

    return contentSize;
}

void TranslationWindow::Resize()
{
    Size parentSize = parentWindow->GetSize();
    descriptor.SetWindowSize(parentSize);

    windowSize.Width = max(parentSize.Width, windowSize.Width);
    windowSize.Height = max(parentSize.Height, windowSize.Height);

    Size bufferingDeviceContextSize = deviceContextBuffer->GetSize();
    bufferingDeviceContextSize.Width = max(parentSize.Width, bufferingDeviceContextSize.Width);
    bufferingDeviceContextSize.Height = max(parentSize.Height, bufferingDeviceContextSize.Height);
    deviceContextBuffer->Resize(bufferingDeviceContextSize);

    AssertCriticalWinApiResult(MoveWindow(windowHandle, position.X, position.Y, windowSize.Width, windowSize.Height, FALSE));

    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    if(translateResult.IsEmptyResult())
    {
        renderer->DrawRect(Rect(0, headerHeight, windowSize.Width, windowSize.Height - headerHeight), disabledBackgroundBrush);
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
    renderer->DrawRect(Rect(0, headerHeight, scaledWidth, height), separatorBrush);
}

TranslationWindow::~TranslationWindow()
{
    AssertCriticalWinApiResult(DeleteObject(separatorBrush));
}