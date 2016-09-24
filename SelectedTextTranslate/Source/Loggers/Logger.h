#pragma once
#include "PrecompiledHeaders\stdafx.h"

class Logger
{
private:
    wstring GetCurrentDateTime() const;
    wstring GetCurrentDate() const;
    wstring GetLocalComputerName() const;
    wstring GetLogFileName() const;

public:
    Logger();
    ~Logger();

    void Log(wstring record);
};