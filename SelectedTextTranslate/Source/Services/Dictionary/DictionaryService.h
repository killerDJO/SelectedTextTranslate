#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Services\Dictionary\Dto\LogRecord.h"

class DictionaryService
{
private:
    Logger* logger;
    sqlite3* database;

    void CreateDatabase() const;

    sqlite3_stmt* CreateStatement(const char* sqlQuery) const;
    void BindValue(sqlite3_stmt* statement, wstring name, wstring value) const;
    void BindValue(sqlite3_stmt* statement, wstring name, int value) const;
    void DictionaryService::ExecuteNonQuery(sqlite3_stmt* statement) const;

    vector<LogRecord> GetLogRecords(sqlite3_stmt* statement) const;

public:
    DictionaryService(Logger* logger);
    ~DictionaryService();

    void UpdateTranslateResult(wstring sentence) const;
    void AddTranslateResult(wstring sentence, wstring json) const;

    vector<LogRecord> GetTopRecords(int topRecordsCount) const;
    vector<LogRecord> GetRecords(wstring sentence) const;
};
