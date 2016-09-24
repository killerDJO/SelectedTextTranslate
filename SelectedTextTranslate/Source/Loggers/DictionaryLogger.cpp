#include "PrecompiledHeaders\stdafx.h"
#include "Loggers\DictionaryLogger.h"
#include "Helpers/StringUtilities.h"

using namespace web;

DictionaryLogger::DictionaryLogger(Logger* logger)
{
    this->logger = logger;

    records = vector<LogRecord>();
    logFileName = L".\\logs\\dictionary_log.json";
    isInitialized = false;
}

void DictionaryLogger::AddRecord(wstring word)
{
    Initialize();

    transform(word.begin(), word.end(), word.begin(), ::tolower);

    for (size_t i = 0; i < records.size(); ++i) 
    {
        if (records[i].Word == word) 
        {
            records[i].Count++;
            Flush();
            return;
        }
    }

    LogRecord newRecord;
    newRecord.Word = word;
    newRecord.Count = 1;
    records.push_back(newRecord);

    Flush();
}

vector<LogRecord> DictionaryLogger::GetRecords()
{
    Initialize();

    sort(records.begin(), records.end(),
        [](const LogRecord & a, const LogRecord & b) -> bool
        {
            return a.Count > b.Count;
        }
    );

    return records;
}

void DictionaryLogger::Flush()
{
    WriteRecords();
}

void DictionaryLogger::Initialize()
{
    if (isInitialized) 
    {
        return;
    }

    ReadRecords();

    isInitialized = true;

    logger->Log(L"Dictionary has been initialized");
}

void DictionaryLogger::ReadRecords()
{
    HANDLE hFile = CreateFile(logFileName, GENERIC_READ, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE) 
    {
        return;
    }

    const int bufferSize = 65536;
    static char buffer[bufferSize];
    wstring json = L"";

    DWORD nWritten;
    do 
    {
        ReadFile(hFile, buffer, bufferSize, &nWritten, nullptr);
        json += wstring((wchar_t*)buffer, nWritten / sizeof(wchar_t));
    } 
    while (nWritten == bufferSize);
    
    CloseHandle(hFile);

    records.resize(0);

    if (json.empty()) {
        return;
    }

    json::value root;

    try 
    {
        root = json::value::parse(json);
    }
    catch (json::json_exception exception)
    {
        logger->Log(L"Error loading dictionary. Dictionary content will be cleared. Exception: " + StringUtilities::GetUtf16String(exception.what()) + L".");
        return;
    }

    if (root.is_null())
    {	
        return;
    }

    for (size_t i = 0; i < root.size(); ++i)
    {
        LogRecord record;
        record.Word = StringUtilities::CopyWideChar(root[i].at(L"word").as_string());
        record.Count = root[i].at(L"count").as_integer();

        records.push_back(record);
    }
}

void DictionaryLogger::WriteRecords()
{
    json::value root;

    for (size_t i = 0; i < DictionaryLogger::records.size(); ++i)
    {
        json::value record;
        record[L"word"] = json::value(StringUtilities::CopyWideChar(DictionaryLogger::records[i].Word));
        record[L"count"] = json::value(DictionaryLogger::records[i].Count);

        root[i] = record;
    }

    wstring json = root.serialize().c_str();

    HANDLE hFile = CreateFile(logFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }

    DWORD nWritten;
    const wchar_t* buffer = json.c_str();
    WriteFile(hFile, buffer, wcslen(buffer) * sizeof(wchar_t), &nWritten, nullptr);

    CloseHandle(hFile);
}

DictionaryLogger::~DictionaryLogger()
{
}