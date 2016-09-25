#include "Windows\Content\Base\ContentWindow.h"

ContentWindow::ContentWindow(WindowContext* context, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel)
    : ChildWindow(context, descriptor, parentWindow),
    fontNormal(nullptr),
    fontHeader(nullptr),
    fontItalic(nullptr),
    fontSmall(nullptr),
    grayBrush(nullptr)
{
    this->appModel = appModel;

    lineHeight = 20;
    paddingX = 15;
    paddingY = 15;
}

void ContentWindow::Initialize()
{
    ChildWindow::Initialize();

    grayBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::LightGray);

    fontNormal = context->GetRenderingContext()->CreateCustomFont(hWindow, FontSizes::Normal);
    fontHeader = context->GetRenderingContext()->CreateCustomFont(hWindow, FontSizes::Large);
    fontItalic = context->GetRenderingContext()->CreateCustomFont(hWindow, FontSizes::Normal, true);
    fontSmall = context->GetRenderingContext()->CreateCustomFont(hWindow, FontSizes::Small);
}

Size ContentWindow::RenderDC(Renderer* renderer)
{
    renderer->ClearDC(inMemoryDC, dcSize);
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