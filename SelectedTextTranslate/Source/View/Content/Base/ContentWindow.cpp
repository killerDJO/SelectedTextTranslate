#include "View\Content\Base\ContentWindow.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

ContentWindow::ContentWindow(WindowContext* context, Window* parentWindow)
    : ChildWindow(context, parentWindow)
{
    this->lineHeight = 20;
    this->paddingX = 15;
    this->paddingY = 15;

    this->fontNormal = context->GetRenderingContext()->CreateCustomFont(FontSizes::Normal);
    this->fontHeader = context->GetRenderingContext()->CreateCustomFont(FontSizes::Large);
    this->fontItalic = context->GetRenderingContext()->CreateCustomFont(FontSizes::Normal, true);
    this->fontSmall = context->GetRenderingContext()->CreateCustomFont(FontSizes::Small);
    this->fontSmallUnderscored = context->GetRenderingContext()->CreateCustomFont(FontSizes::Small, false, true);

    this->lightGrayBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::LightGray);
    this->grayBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::Gray);
    this->backgroundBrush = context->GetRenderingContext()->CreateCustomBrush(Colors::Background);

    this->className = L"STT_CONTENT";
}

int ContentWindow::GetLineHeight() const
{
    return lineHeight;
}

void ContentWindow::SetLineHeight(int lineHeight)
{
    this->lineHeight = lineHeight;
}

int ContentWindow::GetPaddingX() const
{
    return paddingX;
}

void ContentWindow::SetPaddingX(int paddingX)
{
    this->paddingX = paddingX;
}

int ContentWindow::GetPaddingY() const
{
    return paddingY;
}

void ContentWindow::SetPaddingY(int paddingY)
{
    this->paddingY = paddingY;
}

Size ContentWindow::RenderContent(Renderer* renderer)
{
    return Size(0, 0);
}

ContentWindow::~ContentWindow()
{
    AssertCriticalWinApiResult(DeleteObject(fontNormal));
    AssertCriticalWinApiResult(DeleteObject(fontHeader));
    AssertCriticalWinApiResult(DeleteObject(fontItalic));
    AssertCriticalWinApiResult(DeleteObject(fontSmallUnderscored));
    AssertCriticalWinApiResult(DeleteObject(fontSmall));
    AssertCriticalWinApiResult(DeleteObject(lightGrayBrush));
}