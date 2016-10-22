#pragma once

class SelectedTextTranslateBaseException : public runtime_error
{
private:
    wstring callStack;

public:
    SelectedTextTranslateBaseException(wstring message);
    virtual ~SelectedTextTranslateBaseException();

    wstring GetFullErrorMessage() const;
    wstring GetDisplayErrorMessage() const;
};
