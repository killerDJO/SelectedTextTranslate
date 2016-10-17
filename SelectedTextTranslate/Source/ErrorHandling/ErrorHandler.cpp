#include "ErrorHandling\ErrorHandler.h"
#include "ErrorHandling\ExceptionHelper.h"

ErrorHandler::ErrorHandler(Logger* logger)
{
    this->logger = logger;
    OnErrorShow = Subscribeable<>();
}

void ErrorHandler::HandleFatalException()
{
    ExceptionHelper::TerminateOnException(logger);
}

ErrorHandler::~ErrorHandler()
{
}
