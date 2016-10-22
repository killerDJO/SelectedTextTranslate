#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Infrastructure\ErrorHandling\ErrorHandler.h"
#include "Infrastructure\ErrorHandling\Exceptions\Base\SelectedTextTranslateBaseException.h"

#define AssertWinApiResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, false)
#define AssertCriticalWinApiResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, true)
#define AssertWinApiHandleResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, false, INVALID_HANDLE_VALUE)
#define AssertCriticalWinApiHandleResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, true, INVALID_HANDLE_VALUE)

#define AssertGdiPlusResult(__result) ExceptionHelper::ThrowOnGdiPlusError(__result, false)
#define AssertCriticalGdiPlusResult(__result) ExceptionHelper::ThrowOnGdiPlusError(__result, true)

class ExceptionHelper
{
private:
    static void TranslatorFunction(unsigned int, EXCEPTION_POINTERS * eps);
    static wstring GetCurrentExceptionMessage();

public:
    static void ThrowOnWinapiError(bool isFatal = true);
    static void ThrowOnWinapiError(void* resultValue, bool isFatal, void* invalidValue = nullptr);
    static void ThrowOnWinapiError(DWORD resultValue, bool isFatal, DWORD invalidValue = 0);

    static void ThrowOnGdiPlusError(Status status, bool isFatal = true);

    static void SetupStructuredExceptionsTranslation();

    static void HandleNonFatalException(Logger* logger, ErrorHandler* errorHandler, wstring message, const SelectedTextTranslateBaseException& exception);
    static void HandleNonFatalException(Logger* logger, ErrorHandler* errorHandler, wstring message);
    static void TerminateOnException(Logger* logger);
};
