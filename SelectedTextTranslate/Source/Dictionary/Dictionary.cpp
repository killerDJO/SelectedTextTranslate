#include "PrecompiledHeaders\stdafx.h"
#include "Dictionary\Dictionary.h"
#include "Helpers\StringUtilities.h"

Dictionary::Dictionary(Logger* logger)
{
    this->logger = logger;

    int result = sqlite3_open("dictionary.db", &database);

    if (result) {
        logger->Log(L"Can't open dictionary.db: " + StringUtilities::GetUtf16String(sqlite3_errmsg(database)));
        sqlite3_close(database);
        // TODO: throw error.
        return;
    }

    CreateDatabase();
}

void Dictionary::CreateDatabase() const
{
    ExecuteNonQuery(CreateStatement("CREATE TABLE IF NOT EXISTS  Dictionary(Sentence TEXT PRIMARY KEY, Count INTEGER, Json TEXT)"));
    ExecuteNonQuery(CreateStatement("CREATE INDEX IF NOT EXISTS IX_Dictionary_Count on Dictionary(Count)"));
}

vector<LogRecord> Dictionary::GetRecords(wstring sentence) const
{
    sqlite3_stmt* statement = CreateStatement("SELECT Sentence, Count, Json FROM Dictionary WHERE Sentence=@Sentence");
    BindValue(statement, L"@Sentence", sentence);

    vector<LogRecord> logRecords = GetLogRecords(statement);

    return logRecords;
}

void Dictionary::AddTranslateResult(wstring sentence, wstring json) const
{
    sqlite3_stmt* statement = CreateStatement("INSERT INTO Dictionary(Sentence, Count, Json) VALUES(@Sentence, 1, @Json)");
    BindValue(statement, L"@Sentence", sentence);
    BindValue(statement, L"@Json", json);
    ExecuteNonQuery(statement);
}

void Dictionary::UpdateTranslateResult(wstring sentence) const
{
    sqlite3_stmt* statement = CreateStatement("UPDATE Dictionary SET Count = Count + 1 WHERE Sentence = @Sentence");
    BindValue(statement, L"@Sentence", sentence);
    ExecuteNonQuery(statement);
}

vector<LogRecord> Dictionary::GetTopRecords(int topRecordsCount) const
{
    sqlite3_stmt* statement = CreateStatement("SELECT Sentence, Count, Json FROM Dictionary ORDER BY Count DESC LIMIT @Limit");
    BindValue(statement, L"@Limit", topRecordsCount);

    return GetLogRecords(statement);
}

vector<LogRecord> Dictionary::GetLogRecords(sqlite3_stmt* statement) const
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

    sqlite3_finalize(statement);

    return logRecords;
}

sqlite3_stmt* Dictionary::CreateStatement(const char* sqlQuery) const
{
    sqlite3_stmt *statement = nullptr;
    int result = sqlite3_prepare_v2(database, sqlQuery, -1, &statement, nullptr);

    if (result != SQLITE_OK)
    {
        logger->Log(L"SQL error preparing statement. Error code: " + to_wstring(result));
        // TODO: throw
    }

    return statement;
}

void Dictionary::BindValue(sqlite3_stmt* statement, wstring name, wstring value) const
{
    int index = sqlite3_bind_parameter_index(statement, StringUtilities::GetUtf8String(name).c_str());
    int result = sqlite3_bind_text(statement, index, StringUtilities::GetUtf8String(value).c_str(), -1, SQLITE_TRANSIENT);

    if (result != SQLITE_OK)
    {
        logger->Log(L"SQL error binding string value. Error code: " + to_wstring(result));
        // TODO: throw
    }
}

void Dictionary::BindValue(sqlite3_stmt* statement, wstring name, int value) const
{
    int index = sqlite3_bind_parameter_index(statement, StringUtilities::GetUtf8String(name).c_str());
    int result = sqlite3_bind_int(statement, index, value);

    if (result != SQLITE_OK)
    {
        logger->Log(L"SQL error binding int value. Error code: " + to_wstring(result));
        // TODO: throw
    }
}

int Dictionary::ExecuteNonQuery(sqlite3_stmt *statement) const
{
    int result = sqlite3_step(statement);
    while (result != SQLITE_DONE && statement != SQLITE_OK)
    {
        result = sqlite3_step(statement);
    }

    return sqlite3_finalize(statement);
}

Dictionary::~Dictionary()
{
    sqlite3_close(database);
}