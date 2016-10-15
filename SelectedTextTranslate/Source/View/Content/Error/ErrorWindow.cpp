#include "View\Content\Error\ErrorWindow.h"

ErrorWindow::ErrorWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppController* appController)
    : ContentWindow(context, descriptor, parentWindow, appController)
{
}

Size ErrorWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);
    DestroyChildWindows();

    renderer->PrintText(L"Error", fontHeader, Colors::Black, Point(10, 10));

    return renderer->GetScaledSize();
}

ErrorWindow::~ErrorWindow()
{
}