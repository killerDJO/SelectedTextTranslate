#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Utilities\Subscribeable.h"

class ErrorHandler
{
private:
    Logger* logger;

public:
    virtual ~ErrorHandler() = default;
    ErrorHandler(Logger* logger);

    Subscribeable<> OnErrorShow;

    virtual void ShowError(wstring message) = 0;
    virtual void HandleFatalException();
};