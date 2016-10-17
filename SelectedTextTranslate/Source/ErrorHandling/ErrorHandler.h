#pragma once
#include "Logging\Logger.h"
#include "Utilities\Subscribeable.h"

class ErrorHandler
{
private:
    Logger* logger;

public:
    ErrorHandler(Logger* logger);
    virtual ~ErrorHandler();

    Subscribeable<> OnErrorShow;

    virtual void ShowError(wstring message) = 0;
    virtual void HandleFatalException();
};