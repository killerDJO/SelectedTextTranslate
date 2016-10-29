#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Services\Dictionary\Dto\DictionaryRecord.h"
#include "Providers\SqliteProvider.h"

class DictionaryService
{
private:
    Logger* logger;
    SqliteProvider* sqliteProvider;

    sqlite3* database;

    void CreateDatabase() const;

    vector<DictionaryRecord> GetLogRecords(sqlite3_stmt* statement) const;
    
public:
    DictionaryService(Logger* logger, SqliteProvider* sqliteProvider);
    ~DictionaryService();

    void IncrementTranslationsCount(wstring sentence, bool isForcedTranslation) const;

    void UpdateTranslateResult(wstring sentence, wstring json, bool isForcedTranslation) const;
    void AddTranslateResult(wstring sentence, wstring json, bool isForcedTranslation) const;

    vector<DictionaryRecord> GetTopRecords(int topRecordsCount) const;
    bool TryGetCachedRecord(wstring sentence, bool isForcedTranslation, DictionaryRecord& record) const;
};
