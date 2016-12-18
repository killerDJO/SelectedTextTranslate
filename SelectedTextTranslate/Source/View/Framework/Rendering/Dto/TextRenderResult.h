#pragma once
#include "View\Framework\Rendering\Dto\RenderPosition.h"
#include "View\Framework\Dto\Size\SizeReal.h"
#include "View\Framework\Dto\Point\PointReal.h"

class TextRenderResult
{
private:
    SizeReal textSize;
    double baselineY;
    double bottomY;
    double rightX;

public:
    TextRenderResult(SizeReal textSize, double rightX, double baselineY, double bottomY);

    SizeReal GetTextSize() const;
    double GetBaselineY() const;
    double GetBottomY() const;
    double GetRightX() const;
    PointReal GetRight() const;
    RenderPosition GetRenderPosition() const;
};
