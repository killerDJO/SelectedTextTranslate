#include "Infrastructure\ErrorHandling\ErrorHandler.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

ErrorHandler::ErrorHandler(Logger* logger)
{
    this->logger = logger;
    this->OnErrorShow = Subscribeable<>();
}

void ErrorHandler::HandleFatalException()
{
    ExceptionHelper::TerminateOnException(logger);
}
