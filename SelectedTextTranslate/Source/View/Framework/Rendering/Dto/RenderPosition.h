#pragma once

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
    ~RenderPosition();

    Point GetPosition() const;
    int GetY() const;
    int GetX() const;

    RenderPosition SetX(int x) const;
    RenderPosition SetY(int y) const;

    RenderPosition MoveX(int x) const;
    RenderPosition MoveY(int y) const;
};
