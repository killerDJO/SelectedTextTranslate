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

    WindowDescriptor headerWindowDescriptor = WindowDescriptor::CreateWindowDescriptorDownscaled(
        context->GetScaleProvider(),
        PointReal(0, 0),
        SizeReal(0, headerHeight),
        OverflowModes::Stretch,
        OverflowModes::Fixed);
    headerWindow = new HeaderWindow(context, this);
    headerWindow->SetDescriptor(headerWindowDescriptor);
    headerWindow->OnPlayText.Subscribe(&OnPlayText);
    headerWindow->OnTranslateSuggestion.Subscribe(&OnTranslateSuggestion);
    headerWindow->OnForceTranslation.Subscribe(&OnForceTranslation);
    AddChildWindow(headerWindow);

    WindowDescriptor translateResultWindowDescriptor = WindowDescriptor::CreateWindowDescriptorDownscaled(
        context->GetScaleProvider(),
        PointReal(0, headerHeight + 1),
        SizeReal(0, 0),
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

        SizeReal backgroundSize = SizeReal(
            max(parentWindow->GetDownscaledSize().GetWidth(), headerWindow->GetDownscaledSize().GetWidth()),
            parentWindow->GetDownscaledSize().GetHeight() - headerHeight);
        renderer->DrawRect(RectReal(PointReal(0, headerHeight), backgroundSize), backgroundBrush);
        contentSize = headerWindow->GetScaledSize();
    }
    else
    {
        translateResultWindow->MakeVisible();
        translateResultWindow->Render();

        contentSize = Size(
            max(headerWindow->GetScaledSize().GetWidth(), translateResultWindow->GetScaledSize().GetWidth()),
            headerWindow->GetScaledSize().GetHeight() + translateResultWindow->GetScaledSize().GetHeight());
    }

    RenderSeparator(renderer, max(windowSize.GetWidth(), contentSize.GetWidth()));

    return contentSize;
}

void TranslationWindow::Resize()
{
    Size parentSize = parentWindow->GetScaledSize();
    descriptor.SetWindowSize(parentSize);

    windowSize = Size(
        max(parentSize.GetWidth(), windowSize.GetWidth()),
        max(parentSize.GetHeight(), windowSize.GetHeight()));

    Size bufferingDeviceContextSize = deviceContextBuffer->GetSize();
    bufferingDeviceContextSize = Size(
        max(parentSize.GetWidth(), bufferingDeviceContextSize.GetWidth()),
        max(parentSize.GetHeight(), bufferingDeviceContextSize.GetHeight()));
    deviceContextBuffer->Resize(bufferingDeviceContextSize);

    AssertCriticalWinApiResult(MoveWindow(windowHandle, position.GetX(), position.GetY(), windowSize.GetWidth(), windowSize.GetHeight(), FALSE));

    Renderer* renderer = context->GetRenderingContext()->GetRenderer();

    if(model.IsEmptyResult())
    {
        renderer->DrawRect(RectReal(0, headerHeight, windowSize.GetWidth(), windowSize.GetHeight() - headerHeight), lightGrayBrush);
    }

    RenderSeparator(renderer, max(contentSize.GetWidth(), windowSize.GetWidth()));

    renderer->Render(deviceContextBuffer);
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    Draw();
}

void TranslationWindow::RenderSeparator(Renderer* renderer, int width) const
{
    int height = 1;
    double scaledWidth = context->GetScaleProvider()->Downscale(width);
    renderer->DrawRect(RectReal(0, headerHeight, scaledWidth, height), lightGrayBrush);
}

TranslationWindow::~TranslationWindow()
{
}