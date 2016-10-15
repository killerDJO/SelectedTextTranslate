#include "View\Content\Base\ContentWindow.h"

ContentWindow::ContentWindow(WindowContext* context, WindowDescriptor descriptor, Window* parentWindow, AppController* appController)
    : ChildWindow(context, descriptor, parentWindow)
{
    this->appController = appController;

    lineHeight = 20;
    paddingX = 15;
    paddingY = 15;

    this->fontNormal = nullptr;
    this->fontHeader = nullptr;
    this->fontItalic = nullptr;
    this->fontSmall = nullptr;
    this->grayBrush = nullptr;
}

void ContentWindow::Initialize()
{
    ChildWindow::Initialize();

    grayBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::LightGray);

    fontNormal = context->GetRenderingContext()->CreateCustomFont(windowHandle, FontSizes::Normal);
    fontHeader = context->GetRenderingContext()->CreateCustomFont(windowHandle, FontSizes::Large);
    fontItalic = context->GetRenderingContext()->CreateCustomFont(windowHandle, FontSizes::Normal, true);
    fontSmall = context->GetRenderingContext()->CreateCustomFont(windowHandle, FontSizes::Small);
}

Size ContentWindow::RenderContent(Renderer* renderer)
{
    return Size(0, 0);
}

ContentWindow::~ContentWindow()
{
    DeleteObject(fontNormal);
    DeleteObject(fontHeader);
    DeleteObject(fontItalic);
    DeleteObject(fontSmall);
    DeleteObject(grayBrush);
}