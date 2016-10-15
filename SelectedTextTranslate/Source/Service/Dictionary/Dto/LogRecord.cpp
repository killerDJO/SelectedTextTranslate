#include "Service\Dictionary\Dto\LogRecord.h"

LogRecord::LogRecord(wstring word, int count, wstring json)
{
    this->Word = word;
    this->Count = count;
    this->Json = json;
}

LogRecord::~LogRecord()
{
}