#include "View\Framework\Dto\Rendering\TextRenderResult.h"

TextRenderResult::TextRenderResult(Size textSize, int rightX, int baselineY, int bottomY)
{
    this->textSize = textSize;
    this->baselineY = baselineY;
    this->rightX = rightX;
    this->bottomY = bottomY;
}

Size TextRenderResult::GetTextSize() const
{
    return textSize;
}

int TextRenderResult::GetBaselineY() const
{
    return baselineY;
}

int TextRenderResult::GetBottomY() const
{
    return bottomY;
}

int TextRenderResult::GetRightX() const
{
    return rightX;
}

Point TextRenderResult::GetRight() const
{
    return Point(rightX, baselineY);
}

RenderPosition TextRenderResult::GetRenderPosition() const
{
    return RenderPosition(GetRight());
}

TextRenderResult::~TextRenderResult()
{
}
