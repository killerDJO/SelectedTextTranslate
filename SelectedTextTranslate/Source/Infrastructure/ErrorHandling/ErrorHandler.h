#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Utilities\Subscribable\Subscribable.h"

class ErrorHandler
{
private:
    Logger* logger;

public:
    virtual ~ErrorHandler() = default;
    ErrorHandler(Logger* logger);

    Subscribable<> OnErrorShow;

    virtual void ShowError(wstring message) = 0;
    virtual void HandleFatalException();
};