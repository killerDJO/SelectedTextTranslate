#pragma once

class SelectedTextTranslateBaseException : public runtime_error
{
private:
    wstring callStack;

public:
    SelectedTextTranslateBaseException(wstring message);

    wstring GetFullErrorMessage() const;
    wstring GetDisplayErrorMessage() const;
};
