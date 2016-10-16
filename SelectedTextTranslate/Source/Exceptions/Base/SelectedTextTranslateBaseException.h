#pragma once

class SelectedTextTranslateBaseException : public runtime_error
{
private:
    wstring FileName;
    int LineNumber;

public:
    SelectedTextTranslateBaseException(wstring message, const wchar_t* file, unsigned int line);

    wstring GetFullErrorMessage() const;
    wstring GetDisplayErrorMessage() const;
};
