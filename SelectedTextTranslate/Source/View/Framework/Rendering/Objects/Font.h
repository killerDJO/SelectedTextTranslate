#pragma once

class Font
{
private:
    HFONT handle;

    int height;
    int strokeHeight;
    int descent;
    int ascent;

public:
    Font(HFONT handle, int height, int strokeHeight, int descent, int ascent);
    ~Font();

    HFONT GetHandle() const;

    int GetHeight() const;
    int GetStrokeHeight() const;
    int GetDescent() const;
    int GetAscent() const;
};