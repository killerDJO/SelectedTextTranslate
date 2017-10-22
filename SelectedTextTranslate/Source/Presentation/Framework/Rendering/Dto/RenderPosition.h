#pragma once
#include "Presentation\Framework\Dto\Positioning\Point.h"

class ScaleProvider;
class RenderResult;
class TextRenderResult;

class RenderPosition
{
private:
    Point position;

public:
    RenderPosition();
    RenderPosition(Point initialPosition);
    RenderPosition(TextRenderResult textRenderResult);
    RenderPosition(RenderResult renderResult);
    RenderPosition(int x, int y);

    Point GetPosition() const;
    int GetY() const;
    int GetX() const;

    RenderPosition SetX(int x) const;
    RenderPosition SetX(int x, ScaleProvider* scaleProvider) const;
    RenderPosition SetY(int y) const;
    RenderPosition SetY(int y, ScaleProvider* scaleProvider) const;

    RenderPosition MoveX(int x) const;
    RenderPosition MoveX(int x, ScaleProvider* scaleProvider) const;
    RenderPosition MoveY(int y) const;
    RenderPosition MoveY(int y, ScaleProvider* scaleProvider) const;
};
