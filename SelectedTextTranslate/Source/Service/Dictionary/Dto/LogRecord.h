#pragma once

class LogRecord
{
public:
    wstring Word;
    wstring Json;
    int Count;

    LogRecord(wstring word, int count, wstring json);
    ~LogRecord();
};