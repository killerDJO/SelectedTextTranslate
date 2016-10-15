#include "Helpers\ExceptionHelper.h"
#include "Helpers\StringUtilities.h"
#include "Exceptions\SelectedTextTranslateFatalException.h"
#include "Exceptions\SelectedTextTranslateException.h"

void ExceptionHelper::ThrowOnWinapiError(void* resultValue, const wchar_t* file, unsigned int line, bool isFatal, void* invalidValue)
{
    if(resultValue == invalidValue)
    {
        ThrowOnWinapiError(file, line, isFatal);
    }
}

void ExceptionHelper::ThrowOnWinapiError(DWORD resultValue, const wchar_t* file, unsigned line, bool isFatal, DWORD invalidValue)
{
    if (resultValue == invalidValue)
    {
        ThrowOnWinapiError(file, line, isFatal);
    }
}

void ExceptionHelper::ThrowOnWinapiError(const wchar_t* file, unsigned line, bool isFatal)
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