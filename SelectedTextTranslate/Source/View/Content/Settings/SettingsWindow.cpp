#include "View\Content\Settings\SettingsWindow.h"

SettingsWindow::SettingsWindow(WindowContext* context, WindowDescriptor descriptor, wstring name, Window* parentWindow)
    : ContentWindow(context, descriptor, name, parentWindow)
{
}

Size SettingsWindow::RenderContent(Renderer* renderer)
{
    ContentWindow::RenderContent(renderer);
    DestroyChildWindows();

    renderer->PrintText(L"Hello", fontItalic, Colors::Gray, Point(paddingX, lineHeight));

    return renderer->GetScaledSize();
}

SettingsWindow::~SettingsWindow()
{
}