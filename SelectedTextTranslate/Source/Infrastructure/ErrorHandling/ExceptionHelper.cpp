#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Utilities\StringUtilities.h"

void ExceptionHelper::ThrowOnWinapiError(void* resultValue, const wchar_t* file, unsigned int line, bool isFatal, void* invalidValue)
{
    if(resultValue == invalidValue)
    {
        ThrowOnWinapiError(file, line, isFatal);
    }
}

void ExceptionHelper::ThrowOnWinapiError(DWORD resultValue, const wchar_t* file, unsigned int line, bool isFatal, DWORD invalidValue)
{
    if (resultValue == invalidValue)
    {
        ThrowOnWinapiError(file, line, isFatal);
    }
}

void ExceptionHelper::ThrowOnWinapiError(const wchar_t* file, unsigned int line, bool isFatal)
{
    int lastError = GetLastError();
    wstring message = StringUtilities::Format(L"Error calling WINAPI function. Error code: %d.", lastError);

    if (isFatal)
    {
        throw SelectedTextTranslateFatalException(message, file, line);
    }
    else
    {
        throw SelectedTextTranslateException(message, file, line);
    }
}

void ExceptionHelper::ThrowOnGdiPlusError(Status status, const wchar_t* file, unsigned int line, bool isFatal)
{
    if(status != Ok)
    {
        wstring message = StringUtilities::Format(L"Error calling GDI+ function. Status: %d.", status);
        
        if (isFatal)
        {
            throw SelectedTextTranslateFatalException(message, file, line);
        }
        else
        {
            throw SelectedTextTranslateException(message, file, line);
        }
    }
}

wstring ExceptionHelper::GetCurrentExceptionMessage()
{
    exception_ptr exceptionPointer = current_exception();

    if(exceptionPointer == nullptr)
    {
        return wstring();
    }

    try
    {
        rethrow_exception(exceptionPointer);
    }
    catch (const SelectedTextTranslateBaseException& exception)
    {
        return exception.GetFullErrorMessage();
    }
    catch (const exception& exception)
    {
        return StringUtilities::GetUtf16String(exception.what());
    }
    catch (...)
    {
        return L"Unknown exception.";
    }

    return wstring();
}

void ExceptionHelper::SetupStructuredExceptionsTranslation()
{
    _set_se_translator(&TranslatorFunction);
}

void ExceptionHelper::TerminateOnException(Logger* logger)
{
    logger->LogFormatted(L"Unhandled exception occurred. Message: '%ls'.", GetCurrentExceptionMessage().c_str());
    FatalAppExit(0, L"Unhandled exception occurred. See logs for details.");
}

void ExceptionHelper::TranslatorFunction(unsigned int, EXCEPTION_POINTERS * eps)
{
    throw exception(string("SEH exception " + to_string((*eps->ExceptionRecord).ExceptionCode)).c_str());
}
