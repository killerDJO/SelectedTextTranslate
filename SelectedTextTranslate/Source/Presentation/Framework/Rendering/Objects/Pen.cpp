#include "Presentation\Framework\Rendering\Objects\Pen.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

Pen::Pen(HPEN handle)
{
    this->handle = handle;
}

HPEN Pen::GetHandle() const
{
    return handle;
}

Pen::~Pen()
{
    AssertCriticalWinApiResult(DeleteObject(handle));
}