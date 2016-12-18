#include "View\Framework\Rendering\Objects\Font.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

Font::Font(HFONT handle, int height, int strokeHeight, int descent, int ascent)
{
    this->handle = handle;
    this->height = height;
    this->strokeHeight = strokeHeight;
    this->descent = descent;
    this->ascent = ascent;
}

HFONT Font::GetHandle() const
{
    return handle;
}

int Font::GetHeight() const
{
    return height;
}

int Font::GetStrokeHeight() const
{
    return strokeHeight;
}

int Font::GetDescent() const
{
    return descent;
}

int Font::GetAscent() const
{
    return ascent;
}

Font::~Font()
{
    AssertCriticalWinApiResult(DeleteObject(handle));
}
