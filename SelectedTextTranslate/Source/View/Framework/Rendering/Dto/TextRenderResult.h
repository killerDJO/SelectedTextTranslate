#pragma once
#include "View\Framework\Rendering\Dto\RenderPosition.h"

class TextRenderResult
{
private:
    Size textSize;
    int baselineY;
    int bottomY;
    int rightX;

public:
    TextRenderResult(Size textSize, int rightX, int baselineY, int bottomY);
    ~TextRenderResult();

    Size GetTextSize() const;
    int GetBaselineY() const;
    int GetBottomY() const;
    int GetRightX() const;
    Point GetRight() const;
    RenderPosition GetRenderPosition() const;
};
