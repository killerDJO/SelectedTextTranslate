#include "Services\Dictionary\DictionaryService.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Utilities\StringUtilities.h"

DictionaryService::DictionaryService(CompositionRoot* root)
{
    this->logger = root->GetService<Logger>();
    this->sqliteProvider = root->GetService<SqliteProvider>();

    this->database = sqliteProvider->OpenDatabase(L"dictionary.db");

    try
    {
        CreateDatabase();
    }
    catch (const SelectedTextTranslateException& error)
    {
        logger->LogFormatted(LogLevels::Error, L"Error creating database.\n%ls", error.GetFullErrorMessage().c_str());
    }
}

void DictionaryService::CreateDatabase() const
{
    sqliteProvider->ExecuteNonQuery(sqliteProvider->CreateStatement(database, "CREATE TABLE IF NOT EXISTS Dictionary(Sentence TEXT, IsForcedTranslation BOOLEAN, Count INTEGER, Json TEXT, CreatedDate INTEGER, UpdatedDate INTEGER, IsActive BOOLEAN, PRIMARY KEY (Sentence, IsForcedTranslation))"));
    sqliteProvider->ExecuteNonQuery(sqliteProvider->CreateStatement(database, "CREATE INDEX IF NOT EXISTS IX_Dictionary_Count on Dictionary(Count)"));
}

bool DictionaryService::TryGetCachedRecord(wstring sentence, bool isForcedTranslation, DictionaryRecord& logRecord) const
{
    sqlite3_stmt* statement = sqliteProvider->CreateStatement(
        database,
        "SELECT Sentence, Count, Json, IsForcedTranslation, CreatedDate, UpdatedDate, IsActive FROM Dictionary WHERE Sentence=@Sentence AND IsForcedTranslation = @IsForcedTranslation");
    sqliteProvider->BindValue(statement, L"@Sentence", sentence);
    sqliteProvider->BindValue(statement, L"@IsForcedTranslation", isForcedTranslation ? 1 : 0);

    vector<DictionaryRecord> logRecords = GetLogRecords(statement);

    if(logRecords.size() == 0)
    {
        return false;
    }

    logRecord = logRecords[0];
    return true;
}

vector<DictionaryRecord> DictionaryService::GetTopRecords(int topRecordsCount) const
{
    sqlite3_stmt* statement = sqliteProvider->CreateStatement(
        database,
        "SELECT Sentence, Count, Json, IsForcedTranslation, CreatedDate, UpdatedDate, IsActive FROM Dictionary WHERE IsForcedTranslation = 0 ORDER BY Count DESC LIMIT @Limit");
    sqliteProvider->BindValue(statement, L"@Limit", topRecordsCount);

    return GetLogRecords(statement);
}

void DictionaryService::AddTranslateResult(wstring sentence, wstring json, bool isForcedTranslation) const
{
    try
    {
        time_t currentTime = time(nullptr);

        sqlite3_stmt* statement = sqliteProvider->CreateStatement(
            database,
            "INSERT INTO Dictionary(Sentence, Count, Json, IsForcedTranslation, CreatedDate, UpdatedDate, IsActive) VALUES(@Sentence, 0, @Json, @IsForcedTranslation, @CreatedDate, @UpdatedDate, 1)");
        sqliteProvider->BindValue(statement, L"@Sentence", sentence);
        sqliteProvider->BindValue(statement, L"@Json", json);
        sqliteProvider->BindValue(statement, L"@IsForcedTranslation", isForcedTranslation ? 1 : 0);
        sqliteProvider->BindValue(statement, L"@CreatedDate", (long long)currentTime);
        sqliteProvider->BindValue(statement, L"@UpdatedDate", (long long)currentTime);
        sqliteProvider->ExecuteNonQuery(statement);
    }
    catch (const SelectedTextTranslateException& error)
    {
        logger->LogFormatted(LogLevels::Error, L"Error occurred during AddTranslateResult.\n%ls", error.GetFullErrorMessage().c_str());
    }
}

void DictionaryService::UpdateTranslateResult(wstring sentence, wstring json, bool isForcedTranslation) const
{
    try
    {
        time_t currentTime = time(nullptr);

        sqlite3_stmt* statement = sqliteProvider->CreateStatement(
            database,
            "UPDATE Dictionary SET Json = @Json, UpdatedDate = @UpdatedDate WHERE Sentence = @Sentence AND IsForcedTranslation = @IsForcedTranslation");
        sqliteProvider->BindValue(statement, L"@Sentence", sentence);
        sqliteProvider->BindValue(statement, L"@Json", json);
        sqliteProvider->BindValue(statement, L"@IsForcedTranslation", isForcedTranslation ? 1 : 0);
        sqliteProvider->BindValue(statement, L"@UpdatedDate", (long long)currentTime);
        sqliteProvider->ExecuteNonQuery(statement);
    }
    catch (const SelectedTextTranslateException& error)
    {
        logger->LogFormatted(LogLevels::Error, L"Error occurred during UpdateTranslateResult.\n%ls", error.GetFullErrorMessage().c_str());
    }
}

void DictionaryService::IncrementTranslationsCount(wstring sentence, bool isForcedTranslation) const
{
    try
    {
        sqlite3_stmt* statement = sqliteProvider->CreateStatement(
            database,
            "UPDATE Dictionary SET Count = Count + 1 WHERE Sentence = @Sentence AND IsForcedTranslation = @IsForcedTranslation");
        sqliteProvider->BindValue(statement, L"@Sentence", sentence);
        sqliteProvider->BindValue(statement, L"@IsForcedTranslation", isForcedTranslation ? 1 : 0);

        sqliteProvider->ExecuteNonQuery(statement);
    }
    catch (const SelectedTextTranslateException& error)
    {
        logger->LogFormatted(LogLevels::Error, L"Error occurred during UpdateTranslateResult.\n%ls", error.GetFullErrorMessage().c_str());
    }
}

vector<DictionaryRecord> DictionaryService::GetLogRecords(sqlite3_stmt* statement) const
{
    vector<DictionaryRecord> logRecords;

    sqliteProvider->ExecuteReader(statement, [&](map<wstring, int> columnsIndexes) -> void
    {
        const unsigned char* word = sqlite3_column_text(statement, columnsIndexes[L"Sentence"]);
        int count = sqlite3_column_int(statement, columnsIndexes[L"Count"]);
        const unsigned char* json = sqlite3_column_text(statement, columnsIndexes[L"Json"]);
        int isForcedTranslation = sqlite3_column_int(statement, columnsIndexes[L"IsForcedTranslation"]);
        time_t createdDate = sqlite3_column_int64(statement, columnsIndexes[L"CreatedDate"]);
        time_t updatedDate = sqlite3_column_int64(statement, columnsIndexes[L"UpdatedDate"]);
        int isActive = sqlite3_column_int(statement, columnsIndexes[L"IsActive"]);

        DictionaryRecord logRecord(
            StringUtilities::GetUtf16StringFromChar((char*)word),
            isForcedTranslation == 1,
            count,
            StringUtilities::GetUtf16StringFromChar((char*)json),
            createdDate,
            updatedDate,
            isActive == 1);

        logRecords.push_back(logRecord);
    });

    return logRecords;
}

DictionaryService::~DictionaryService()
{
    sqlite3_close(database);
}