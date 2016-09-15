#include "Windows\Content\Base\ContentWindow.h"

ContentWindow::ContentWindow(Renderer* renderer, AppModel* appModel, HWND parentWindow, HINSTANCE hInstance, DWORD x, DWORD y, DWORD width, DWORD height)
    : ChildWindow(renderer, parentWindow, hInstance, x, y, width, height)
{
    this->appModel = appModel;

    this->dcHeight = 0;
    this->dcWidth = 0;

    lineHeight = renderer->AdjustToYResolution(20);
    paddingX = renderer->AdjustToXResolution(15);
    paddingY = renderer->AdjustToYResolution(15);
}

void ContentWindow::Initialize()
{
    ChildWindow::Initialize();

    grayBrush = renderer->CreateCustomBrush(Colors::LightGray);

    fontNormal = renderer->CreateCustomFont(hWindow, FontSizes::Normal);
    fontHeader = renderer->CreateCustomFont(hWindow, FontSizes::Large);
    fontItalic = renderer->CreateCustomFont(hWindow, FontSizes::Normal, true);
    fontSmall = renderer->CreateCustomFont(hWindow, FontSizes::Small);
}

void ContentWindow::InitializeInMemoryDC()
{
    inMemoryDC = renderer->CreateInMemoryDC(dcWidth, dcHeight);
}

POINT ContentWindow::RenderResult()
{
    POINT bottomRight = RenderDC();

    // Check if current buffer DC has enough size
    if (bottomRight.x > (int)dcWidth || bottomRight.y > (int)dcHeight)
    {
        dcWidth = bottomRight.x;
        dcHeight = bottomRight.y;
        renderer->ResizeDC(inMemoryDC, dcWidth, dcHeight);
        RenderDC();
    }

    width = bottomRight.x;
    height = bottomRight.y;

    InvalidateRect(hWindow, NULL, FALSE);
    MoveWindow(hWindow, x, y, width, height, FALSE);
    return bottomRight;
}

POINT ContentWindow::RenderDC()
{
    POINT bottomRight = ChildWindow::RenderDC();
    renderer->ClearDC(inMemoryDC, dcWidth, dcHeight);
    return bottomRight;
}

ContentWindow::~ContentWindow()
{
    DeleteObject(fontNormal);
    DeleteObject(fontHeader);
    DeleteObject(fontItalic);
    DeleteObject(fontSmall);
    DeleteObject(grayBrush);
}