#pragma once

class ExceptionHelper
{
private:
    static void ThrowOnWinapiError(const wchar_t* file, unsigned int line, bool isFatal = true);

public:
    static void ThrowOnWinapiError(void* resultValue, const wchar_t* file, unsigned int line, bool isFatal, void* invalidValue = nullptr);
    static void ThrowOnWinapiError(DWORD resultValue, const wchar_t* file, unsigned int line, bool isFatal);

    static void ThrowFromStdException(std::exception exception, const wchar_t* file, unsigned int line);
};