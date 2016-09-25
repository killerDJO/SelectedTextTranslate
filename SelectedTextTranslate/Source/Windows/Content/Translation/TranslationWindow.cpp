#include "Windows\Content\Translation\TranslationWindow.h"
#include "Windows\Content\Translation\HeaderWindow.h"
#include "Windows\Content\Translation\TranslateResultWindow.h"

TranslationWindow::TranslationWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel)
    : ContentWindow(context, descriptor, parentWindow, appModel)
{
    separatorBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::LightGray);
}

Size TranslationWindow::RenderDC(Renderer* renderer)
{
    ContentWindow::RenderDC(renderer);

    WindowDescriptor headerWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, 0),
        Size(0, headerHeight),
        OverflowModes::Stretch,
        OverflowModes::Fixed);
    HeaderWindow* headerWindow = new HeaderWindow(context, headerWindowDescriptor, hWindow, appModel);
    AddChildWindow(headerWindow);

    WindowDescriptor translateResultWindowDescriptor = WindowDescriptor::CreateWindowDescriptor(
        Point(0, headerHeight + 1),
        Size(0, 0),
        OverflowModes::Stretch,
        OverflowModes::Stretch);
    TranslateResultWindow* translateResultWindow = new TranslateResultWindow(context, translateResultWindowDescriptor, hWindow, appModel);
    AddChildWindow(translateResultWindow);

    headerWindow->Render();
    translateResultWindow->Render();

    Size contentSize;
    contentSize.Width = max(headerWindow->GetWidth(), translateResultWindow->GetWidth());
    contentSize.Height = headerWindow->GetHeight() + translateResultWindow->GetHeight();

    RenderSeparator(renderer, max(contentSize.Width, windowSize.Width));

    return contentSize;
}

void TranslationWindow::RenderSeparator(Renderer* renderer, int width)
{
    //int width = windowSize.Width;
    int height = 1;
    //int verticalScrollOffset = context->GetScrollProvider()->GetCurrentOffset(this, ScrollBars::Vertical);
    renderer->DrawRect(Rect(0, headerHeight, width, height), separatorBrush);
}

TranslationWindow::~TranslationWindow()
{
    DeleteObject(separatorBrush);
}