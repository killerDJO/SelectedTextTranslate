#include "Presentation\Framework\Views\ContentView.h"

ContentView::ContentView(CommonContext* context, View* parentView)
    : ChildView(context, parentView)
{
    this->lineHeight = scaleProvider->Scale(20);
    this->paddingX = scaleProvider->Scale(15);
    this->paddingY = scaleProvider->Scale(15);

    this->fontNormal = renderingProvider->CreateCustomFont(FontSizes::Normal);
    this->fontHeader = renderingProvider->CreateCustomFont(FontSizes::Large);
    this->fontItalic = renderingProvider->CreateCustomFont(FontSizes::Normal, true);
    this->fontSmall = renderingProvider->CreateCustomFont(FontSizes::Small);
    this->fontSmallUnderscored = renderingProvider->CreateCustomFont(FontSizes::Small, false, true);

    this->lightGrayBrush = renderingProvider->CreateCustomBrush(Colors::LightGray);
    this->grayBrush = renderingProvider->CreateCustomBrush(Colors::Gray);
    this->backgroundBrush = renderingProvider->CreateCustomBrush(Colors::Background);

    this->className = L"STT_CONTENT";
}

void ContentView::Render(bool preserveScrolls)
{
    bool isStretchWindow = descriptor.GetOverflowX() == OverflowModes::Stretch || descriptor.GetOverflowY() == OverflowModes::Stretch;
    if (isStretchWindow && !renderingContext->IsRenderingInProgress())
    {
        parentView->Render(preserveScrolls);
    }
    else
    {
        ChildView::Render(preserveScrolls);
    }
}

int ContentView::GetLineHeight() const
{
    return lineHeight;
}

void ContentView::SetLineHeight(int lineHeight)
{
    this->lineHeight = lineHeight;
}

int ContentView::GetPaddingX() const
{
    return paddingX;
}

void ContentView::SetPaddingX(int paddingX)
{
    this->paddingX = paddingX;
}

int ContentView::GetPaddingY() const
{
    return paddingY;
}

void ContentView::SetPaddingY(int paddingY)
{
    this->paddingY = paddingY;
}

ContentView::~ContentView()
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