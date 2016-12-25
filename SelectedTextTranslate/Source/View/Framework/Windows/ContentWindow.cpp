#include "View\Framework\Windows\ContentWindow.h"

ContentWindow::ContentWindow(WindowContext* context, Window* parentWindow)
    : ChildWindow(context, parentWindow)
{
    this->lineHeight = context->GetScaleProvider()->Scale(20);
    this->paddingX = context->GetScaleProvider()->Scale(15);
    this->paddingY = context->GetScaleProvider()->Scale(15);

    this->fontNormal = context->GetRenderingProvider()->CreateCustomFont(FontSizes::Normal);
    this->fontHeader = context->GetRenderingProvider()->CreateCustomFont(FontSizes::Large);
    this->fontItalic = context->GetRenderingProvider()->CreateCustomFont(FontSizes::Normal, true);
    this->fontSmall = context->GetRenderingProvider()->CreateCustomFont(FontSizes::Small);
    this->fontSmallUnderscored = context->GetRenderingProvider()->CreateCustomFont(FontSizes::Small, false, true);

    this->lightGrayBrush = context->GetRenderingProvider()->CreateCustomBrush(Colors::LightGray);
    this->grayBrush = context->GetRenderingProvider()->CreateCustomBrush(Colors::Gray);
    this->backgroundBrush = context->GetRenderingProvider()->CreateCustomBrush(Colors::Background);

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

ContentWindow::~ContentWindow()
{
    delete fontNormal;
    delete fontHeader;
    delete fontItalic;
    delete fontSmallUnderscored;
    delete fontSmall;
    delete lightGrayBrush;
    delete grayBrush;
    delete backgroundBrush;
}