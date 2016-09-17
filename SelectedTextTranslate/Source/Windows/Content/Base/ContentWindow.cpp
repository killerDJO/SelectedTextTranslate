#include "Windows\Content\Base\ContentWindow.h"

ContentWindow::ContentWindow(HINSTANCE hInstance, RenderingContext* renderingContext, ScrollProvider* scrollProvider, WindowDescriptor descriptor, HWND parentWindow, AppModel* appModel)
    : ChildWindow(hInstance, renderingContext, scrollProvider, descriptor, parentWindow)
{
    this->appModel = appModel;

    lineHeight = 20;
    paddingX = 15;
    paddingY = 15;
}

void ContentWindow::Initialize()
{
    ChildWindow::Initialize();

    grayBrush = renderingContext->CreateCustomBrush(Colors::LightGray);

    fontNormal = renderingContext->CreateCustomFont(hWindow, FontSizes::Normal);
    fontHeader = renderingContext->CreateCustomFont(hWindow, FontSizes::Large);
    fontItalic = renderingContext->CreateCustomFont(hWindow, FontSizes::Normal, true);
    fontSmall = renderingContext->CreateCustomFont(hWindow, FontSizes::Small);
}

SIZE ContentWindow::RenderDC(Renderer* renderer)
{
    renderingContext->ClearDC(inMemoryDC, dcWidth, dcHeight);
    return SIZE();
}

ContentWindow::~ContentWindow()
{
    DeleteObject(fontNormal);
    DeleteObject(fontHeader);
    DeleteObject(fontItalic);
    DeleteObject(fontSmall);
    DeleteObject(grayBrush);
}