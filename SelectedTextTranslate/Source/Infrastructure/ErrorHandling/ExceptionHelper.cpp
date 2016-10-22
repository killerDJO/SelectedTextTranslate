#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Utilities\StringUtilities.h"
#include "Exceptions\SelectedTextTranslateException.h"
#include "Exceptions\SelectedTextTranslateFatalException.h"

void ExceptionHelper::ThrowOnWinapiError(void* resultValue, bool isFatal, void* invalidValue)
{
    if(resultValue == invalidValue)
    {
        ThrowOnWinapiError(isFatal);
    }
}

void ExceptionHelper::ThrowOnWinapiError(DWORD resultValue,  bool isFatal, DWORD invalidValue)
{
    if (resultValue == invalidValue)
    {
        ThrowOnWinapiError(isFatal);
    }
}

void ExceptionHelper::ThrowOnWinapiError(bool isFatal)
{
    int lastError = GetLastError();
    wstring message = StringUtilities::Format(L"Error calling WINAPI function. Error code: %d.", lastError);

    if (isFatal)
    {
        throw SelectedTextTranslateFatalException(message);
    }
    else
    {
        throw SelectedTextTranslateException(message);
    }
}

void ExceptionHelper::ThrowOnGdiPlusError(Status status, bool isFatal)
{
    if(status != Ok)
    {
        wstring message = StringUtilities::Format(L"Error calling GDI+ function. Status: %d.", status);
        
        if (isFatal)
        {
            throw SelectedTextTranslateFatalException(message);
        }
        else
        {
            throw SelectedTextTranslateException(message);
        }
    }
}

wstring ExceptionHelper::GetCurrentExceptionMessage()
{
    wstring exceptionMessage;
    exception_ptr exceptionPointer = current_exception();

    if(exceptionPointer == nullptr)
    {
        exceptionMessage = wstring();
    }

    try
    {
        rethrow_exception(exceptionPointer);
    }
    catch (const SelectedTextTranslateBaseException& exception)
    {
        exceptionMessage = exception.GetFullErrorMessage();
    }
    catch (const exception& exception)
    {
        exceptionMessage = StringUtilities::Format(L"\tException message: '%hs'", exception.what());
    }
    catch (...)
    {
        exceptionMessage = L"\tUnknown exception occurred.";
    }

    return exceptionMessage;
}

void ExceptionHelper::SetupStructuredExceptionsTranslation()
{
    _set_se_translator(&TranslatorFunction);
}

void ExceptionHelper::HandleNonFatalException(Logger* logger, ErrorHandler* errorHandler, wstring message, const SelectedTextTranslateBaseException& exception)
{
    logger->LogFormatted(L"%ls\n%ls", message.c_str(), exception.GetFullErrorMessage().c_str());
    errorHandler->ShowError(exception.GetDisplayErrorMessage());
}

void ExceptionHelper::HandleNonFatalException(Logger* logger, ErrorHandler* errorHandler, wstring message)
{
    logger->LogFormatted(L"%ls\n%ls", message.c_str(), GetCurrentExceptionMessage().c_str());
    errorHandler->ShowError(message);
}

void ExceptionHelper::TerminateOnException(Logger* logger)
{
    logger->LogFormatted(L"Unhandled exception occurred.\n%ls", GetCurrentExceptionMessage().c_str());
    FatalAppExit(0, L"Unhandled exception occurred. See logs for details.");
}

void ExceptionHelper::TranslatorFunction(unsigned int, EXCEPTION_POINTERS * eps)
{
    throw exception(string("SEH exception " + to_string((*eps->ExceptionRecord).ExceptionCode)).c_str());
}
