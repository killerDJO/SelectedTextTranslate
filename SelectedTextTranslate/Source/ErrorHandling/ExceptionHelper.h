#pragma once
#include "Logging\Logger.h"
#include "ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"
#include "ErrorHandling\Exceptions\SelectedTextTranslateException.h"

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)

#define AssertWinApiResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, __WFILE__, __LINE__, false)
#define AssertCriticalWinApiResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, __WFILE__, __LINE__, true)
#define AssertWinApiHandleResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, __WFILE__, __LINE__, false, INVALID_HANDLE_VALUE)
#define AssertCriticalWinApiHandleResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, __WFILE__, __LINE__, true, INVALID_HANDLE_VALUE)

#define AssertGdiPlusResult(__result) ExceptionHelper::ThrowOnGdiPlusError(__result, __WFILE__, __LINE__, false)
#define AssertCriticalGdiPlusResult(__result) ExceptionHelper::ThrowOnGdiPlusError(__result, __WFILE__, __LINE__, true)

#define ThrowSelectedTextTranslateException(message) throw SelectedTextTranslateException(message, __WFILE__, __LINE__)
#define ThrowSelectedTextTranslateFatalException(message) throw SelectedTextTranslateFatalException(message, __WFILE__, __LINE__)

class ExceptionHelper
{
private:
    static void TranslatorFunction(unsigned int, EXCEPTION_POINTERS * eps);

public:
    static void ThrowOnWinapiError(const wchar_t* file, unsigned int line, bool isFatal = true);
    static void ThrowOnWinapiError(void* resultValue, const wchar_t* file, unsigned int line, bool isFatal, void* invalidValue = nullptr);
    static void ThrowOnWinapiError(DWORD resultValue, const wchar_t* file, unsigned int line, bool isFatal, DWORD invalidValue = 0);

    static void ThrowOnGdiPlusError(Status status, const wchar_t* file, unsigned int line, bool isFatal = true);

    static wstring GetCurrentExceptionMessage();
    static void SetupStructuredExceptionsTranslation();

    static void TerminateOnException(Logger* logger);
};