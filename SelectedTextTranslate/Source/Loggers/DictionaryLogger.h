#pragma once
#include "Entities\LogRecord.h"
#include "Loggers\Logger.h"

class DictionaryLogger 
{
private:
    Logger* logger;

    vector<LogRecord> records;
    const wchar_t* logFileName;
    bool isInitialized;

    void Initialize();
    void ReadRecords();
    void WriteRecords();

public:
    DictionaryLogger(Logger* logger);
    ~DictionaryLogger();

    void AddRecord(wstring word);
    vector<LogRecord> GetRecords();
    void Flush();
};