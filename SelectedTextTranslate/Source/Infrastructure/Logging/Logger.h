#pragma once
#include "Infrastructure\Logging\Enums\LogLevels.h"

class Logger
{
private:
    map<LogLevels, wstring> logLevelsDisplayMap;

    wstring GetCurrentDateTime() const;
    wstring GetCurrentDate() const;
    wstring GetLocalComputerName() const;
    wstring GetLogFileName() const;

public:
    Logger();
    ~Logger();

    void Log(LogLevels logLevel, wstring record);
    void LogFormatted(LogLevels logLevel, wstring format, ...);
};