#include "View\Framework\Rendering\Dto\TextRenderResult.h"

TextRenderResult::TextRenderResult(SizeReal textSize, double rightX, double baselineY, double bottomY)
{
    this->textSize = textSize;
    this->baselineY = baselineY;
    this->rightX = rightX;
    this->bottomY = bottomY;
}

SizeReal TextRenderResult::GetTextSize() const
{
    return textSize;
}

double TextRenderResult::GetBaselineY() const
{
    return baselineY;
}

double TextRenderResult::GetBottomY() const
{
    return bottomY;
}

double TextRenderResult::GetRightX() const
{
    return rightX;
}

PointReal TextRenderResult::GetRight() const
{
    return PointReal(rightX, baselineY);
}

RenderPosition TextRenderResult::GetRenderPosition() const
{
    return RenderPosition(GetRight());
}
