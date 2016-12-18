#pragma once
#include "View\Framework\Dto\Point\PointReal.h"

class RenderResult;
class TextRenderResult;

class RenderPosition
{
private:
    PointReal position;

public:
    RenderPosition();
    RenderPosition(PointReal initialPosition);
    RenderPosition(TextRenderResult textRenderResult);
    RenderPosition(RenderResult renderResult);
    RenderPosition(double x, double y);

    PointReal GetPosition() const;
    double GetY() const;
    double GetX() const;

    RenderPosition SetX(double x) const;
    RenderPosition SetY(double y) const;

    RenderPosition MoveX(double x) const;
    RenderPosition MoveY(double y) const;
};
