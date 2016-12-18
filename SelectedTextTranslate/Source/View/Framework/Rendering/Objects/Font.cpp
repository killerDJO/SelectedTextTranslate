#include "View\Framework\Rendering\Objects\Font.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

Font::Font(HFONT handle)
{
    this->handle = handle;
}

HFONT Font::GetHandle() const
{
    return handle;
}

Font::~Font()
{
    AssertCriticalWinApiResult(DeleteObject(handle));
}