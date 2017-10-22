#pragma once
#include "Presentation\Framework\Rendering\Dto\RenderPosition.h"
#include "Presentation\Framework\Dto\Positioning\Size.h"
#include "Presentation\Framework\Dto\Positioning\Point.h"

class TextRenderResult
{
private:
    Size textSize;
    int baselineY;
    int bottomY;
    int rightX;

public:
    TextRenderResult(Size textSize, int rightX, int baselineY, int bottomY);

    Size GetTextSize() const;
    int GetBaselineY() const;
    int GetBottomY() const;
    int GetRightX() const;
    Point GetRight() const;
    RenderPosition GetRenderPosition() const;
};
