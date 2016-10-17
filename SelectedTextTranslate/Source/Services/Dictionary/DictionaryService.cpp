#include "Services\Dictionary\DictionaryService.h"
#include "Utilities\StringUtilities.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

DictionaryService::DictionaryService(Logger* logger)
{
    this->logger = logger;

    int result = sqlite3_open("dictionary.db", &database);

    if (result)
    {
        sqlite3_close(database);
        wstring message = StringUtilities::Format(L"Can't open dictionary.db: %ls", StringUtilities::GetUtf16String(sqlite3_errmsg(database)).c_str());
        ThrowSelectedTextTranslateException(message);
    }

    CreateDatabase();
}

void DictionaryService::CreateDatabase() const
{
    ExecuteNonQuery(CreateStatement("CREATE TABLE IF NOT EXISTS  Dictionary(Sentence TEXT PRIMARY KEY, Count INTEGER, Json TEXT)"));
    ExecuteNonQuery(CreateStatement("CREATE INDEX IF NOT EXISTS IX_Dictionary_Count on Dictionary(Count)"));
}

vector<LogRecord> DictionaryService::GetRecords(wstring sentence) const
{
    sqlite3_stmt* statement = CreateStatement("SELECT Sentence, Count, Json FROM Dictionary WHERE Sentence=@Sentence");
    BindValue(statement, L"@Sentence", sentence);

    vector<LogRecord> logRecords = GetLogRecords(statement);

    return logRecords;
}

void DictionaryService::AddTranslateResult(wstring sentence, wstring json) const
{
    try
    {
        sqlite3_stmt* statement = CreateStatement("INSERT INTO Dictionary(Sentence, Count, Json) VALUES(@Sentence, 1, @Json)");
        BindValue(statement, L"@Sentence", sentence);
        BindValue(statement, L"@Json", json);
        ExecuteNonQuery(statement);
    }
    catch (const SelectedTextTranslateException& error)
    {
        logger->LogFormatted(L"Error occurred during AddTranslateResult. Message: '%ls'.", error.GetFullErrorMessage().c_str());
    }
}

void DictionaryService::UpdateTranslateResult(wstring sentence) const
{
    try
    {
        sqlite3_stmt* statement = CreateStatement("UPDATE Dictionary SET Count = Count + 1 WHERE Sentence = @Sentence");
        BindValue(statement, L"@Sentence", sentence);
        ExecuteNonQuery(statement);
    }
    catch (const SelectedTextTranslateException& error)
    {
        logger->LogFormatted(L"Error occurred during UpdateTranslateResult. Message: '%ls'.", error.GetFullErrorMessage().c_str());
    }
}

vector<LogRecord> DictionaryService::GetTopRecords(int topRecordsCount) const
{
    sqlite3_stmt* statement = CreateStatement("SELECT Sentence, Count, Json FROM Dictionary ORDER BY Count DESC LIMIT @Limit");
    BindValue(statement, L"@Limit", topRecordsCount);

    return GetLogRecords(statement);
}

vector<LogRecord> DictionaryService::GetLogRecords(sqlite3_stmt* statement) const
{
    vector<LogRecord> logRecords;

    int result = sqlite3_step(statement);
    while (result != SQLITE_DONE && result != SQLITE_OK)
    {
        const unsigned char * word = sqlite3_column_text(statement, 0);
        int count = sqlite3_column_int(statement, 1);
        const unsigned char * json = sqlite3_column_text(statement, 2);

        LogRecord logRecord(
            StringUtilities::GetUtf16StringFromChar((char*)word),
            count,
            StringUtilities::GetUtf16StringFromChar((char*)json));

        logRecords.push_back(logRecord);

        result = sqlite3_step(statement);
    }

    result = sqlite3_finalize(statement);

    if (result != SQLITE_OK)
    {
        wstring message = StringUtilities::Format(L"SQL error getting log records. Error code: %d", result);
        ThrowSelectedTextTranslateException(message);
    }

    return logRecords;
}

sqlite3_stmt* DictionaryService::CreateStatement(const char* sqlQuery) const
{
    sqlite3_stmt *statement = nullptr;
    int result = sqlite3_prepare_v2(database, sqlQuery, -1, &statement, nullptr);

    if (result != SQLITE_OK)
    {
        wstring message = StringUtilities::Format(L"SQL error preparing statement. Error code: %d", result);
        ThrowSelectedTextTranslateException(message);
    }

    return statement;
}

void DictionaryService::BindValue(sqlite3_stmt* statement, wstring name, wstring value) const
{
    int index = sqlite3_bind_parameter_index(statement, StringUtilities::GetUtf8String(name).c_str());
    int result = sqlite3_bind_text(statement, index, StringUtilities::GetUtf8String(value).c_str(), -1, SQLITE_TRANSIENT);

    if (result != SQLITE_OK)
    {
        wstring message = StringUtilities::Format(L"SQL error binding string value. Error code: %d", result);
        ThrowSelectedTextTranslateException(message);
    }
}

void DictionaryService::BindValue(sqlite3_stmt* statement, wstring name, int value) const
{
    int index = sqlite3_bind_parameter_index(statement, StringUtilities::GetUtf8String(name).c_str());
    int result = sqlite3_bind_int(statement, index, value);

    if (result != SQLITE_OK)
    {
        wstring message = StringUtilities::Format(L"SQL error binding int value. Error code: %d", result);
        ThrowSelectedTextTranslateException(message);
    }
}

void DictionaryService::ExecuteNonQuery(sqlite3_stmt *statement) const
{
    int result = sqlite3_step(statement);
    while (result != SQLITE_DONE && statement != SQLITE_OK)
    {
        result = sqlite3_step(statement);
    }

    result = sqlite3_finalize(statement);

    if(result != SQLITE_OK)
    {
        wstring message = StringUtilities::Format(L"SQL error executing non query. Error code: %d", result);
        ThrowSelectedTextTranslateException(message);
    }
}

DictionaryService::~DictionaryService()
{
    sqlite3_close(database);
}