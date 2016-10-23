#pragma once

class LogRecord
{
public:
    wstring Word;
    bool IsForcedTranslation;
    wstring Json;
    int Count;
    time_t CreatedDate;
    time_t UpdatedDate;
    bool IsActive;

    LogRecord(wstring word, bool isForcedTranslation, int count, wstring json, time_t createdDate, time_t updatedDate, bool isActive);
    LogRecord();
    ~LogRecord();
};