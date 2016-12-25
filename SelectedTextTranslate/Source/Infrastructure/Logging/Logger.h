#pragma once
#include "Infrastructure\Logging\Enums\LogLevels.h"

using namespace concurrency;

class Logger
{
private:
    const UINT WriterRecordsMessage = 42;

    map<LogLevels, wstring> logLevelsDisplayMap;
    DWORD writerThreadId;

    wstring GetCurrentDateTime() const;
    wstring GetCurrentDate() const;
    wstring GetLocalComputerName() const;
    wstring GetLogFileName() const;

    concurrent_queue<wstring> loggingQueue;

    static DWORD WINAPI WriterThread(LPVOID arg);
    void WriteRecords();

public:
    Logger();

    void Log(LogLevels logLevel, wstring record);
    void LogFormatted(LogLevels logLevel, wstring format, ...);
};