#include "View\Framework\Rendering\Objects\Brush.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

Brush::Brush(HBRUSH handle)
{
    this->handle = handle;
}

HBRUSH Brush::GetHandle() const
{
    return handle;
}

Brush::~Brush()
{
    AssertCriticalWinApiResult(DeleteObject(handle));
}