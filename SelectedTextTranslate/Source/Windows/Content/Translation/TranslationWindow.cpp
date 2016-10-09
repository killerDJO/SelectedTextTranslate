#include "Windows\Content\Translation\TranslationWindow.h"

TranslationWindow::TranslationWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppModel* appModel)
    : ContentWindow(context, descriptor, parentWindow, appModel)
{
    separatorBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::LightGray);
}

void TranslationWindow::Initialize()
{
    ContentWindow::Initialize();

    WindowDescriptor headerWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, 0),
        Size(0, headerHeight),
        OverflowModes::Stretch,
        OverflowModes::Fixed);
    headerWindow = new HeaderWindow(context, headerWindowDescriptor, this, appModel);
    AddChildWindow(headerWindow);

    WindowDescriptor translateResultWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, headerHeight + 1),
        Size(0, 0),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
    translateResultWindow = new TranslateResultWindow(context, translateResultWindowDescriptor, this, appModel);
    AddChildWindow(translateResultWindow);
}

Size TranslationWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);

    headerWindow->Render();
    translateResultWindow->Render();

    Size contentSize;
    contentSize.Width = max(headerWindow->GetSize().Width, translateResultWindow->GetSize().Width);
    contentSize.Height = headerWindow->GetSize().Height + translateResultWindow->GetSize().Height;

    RenderSeparator(renderer, max(windowSize.Width, contentSize.Width));

    return contentSize;
}

void TranslationWindow::Resize()
{
    Size parentClientSize = parentWindow->GetAvailableClientSize();
    descriptor.WindowSize = parentClientSize;

    windowSize.Width = max(parentClientSize.Width, windowSize.Width);
    windowSize.Height = max(parentClientSize.Height, windowSize.Height);

    Size bufferingDeviceContextSize = deviceContextBuffer->GetSize();
    bufferingDeviceContextSize.Width = max(parentClientSize.Width, bufferingDeviceContextSize.Width);
    bufferingDeviceContextSize.Height = max(parentClientSize.Height, bufferingDeviceContextSize.Height);
    deviceContextBuffer->Resize(bufferingDeviceContextSize);

    MoveWindow(windowHandle, descriptor.Position.X, descriptor.Position.Y, windowSize.Width, windowSize.Height, FALSE);

    Renderer* renderer = context->GetRenderingContext()->GetRenderer();
    RenderSeparator(renderer, max(contentSize.Width, windowSize.Width));
    renderer->Render(deviceContextBuffer);
    context->GetRenderingContext()->ReleaseRenderer(renderer);

    ForceDraw();
}

void TranslationWindow::RenderSeparator(Renderer* renderer, int width) const
{
    int height = 1;
    int scaledWidth = context->GetScaleProvider()->Downscale(width);
    renderer->DrawRect(Rect(0, headerHeight, scaledWidth, height), separatorBrush);
}

TranslationWindow::~TranslationWindow()
{
    DeleteObject(separatorBrush);
}