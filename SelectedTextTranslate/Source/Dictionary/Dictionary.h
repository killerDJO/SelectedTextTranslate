#pragma once
#include "Loggers\Logger.h"
#include "Dictionary\Dto\LogRecord.h"

class Dictionary
{
private:
    Logger* logger;
    sqlite3* database;

    void CreateDatabase() const;

    sqlite3_stmt* CreateStatement(const char* sqlQuery) const;
    void BindValue(sqlite3_stmt* statement, wstring name, wstring value) const;
    void BindValue(sqlite3_stmt* statement, wstring name, int value) const;
    int Dictionary::ExecuteNonQuery(sqlite3_stmt* statement) const;

    vector<LogRecord> GetLogRecords(sqlite3_stmt* statement) const;

public:
    Dictionary(Logger* logger);
    ~Dictionary();

    void UpdateTranslateResult(wstring sentence) const;
    void AddTranslateResult(wstring sentence, wstring json) const;
    vector<LogRecord> GetTopRecords(int topRecordsCount) const;
    vector<LogRecord> GetRecords(wstring sentence) const;
};
