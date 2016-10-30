#include "Providers\SqliteProvider.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Utilities\StringUtilities.h"

SqliteProvider::SqliteProvider()
{
}

sqlite3* SqliteProvider::OpenDatabase(wstring databaseName) const
{
    sqlite3* database;
    int result = sqlite3_open("dictionary.db", &database);

    if (result)
    {
        sqlite3_close(database);
        wstring message = StringUtilities::Format(L"Can't open %ls: %ls", databaseName.c_str(), StringUtilities::GetUtf16String(sqlite3_errmsg(database)).c_str());
        throw SelectedTextTranslateException(message);
    }

    ApplyPragmaStatement(database, "PRAGMA synchronous = OFF");
    ApplyPragmaStatement(database, "PRAGMA journal_mode = MEMORY");

    return database;
}

void SqliteProvider::ApplyPragmaStatement(sqlite3* database, string statement) const
{
    char* errorMessage;
    sqlite3_exec(database, statement.c_str(), nullptr, nullptr, &errorMessage);

    if(errorMessage != nullptr)
    {
        wstring message = StringUtilities::Format(L"Unable to apply PRAGMA statement. Error: %hs", errorMessage);
        sqlite3_free(errorMessage);
        throw SelectedTextTranslateException(message);
    }
}

sqlite3_stmt* SqliteProvider::CreateStatement(sqlite3* database, const char* sqlQuery) const
{
    sqlite3_stmt *statement = nullptr;
    int result = sqlite3_prepare_v2(database, sqlQuery, -1, &statement, nullptr);

    if (result != SQLITE_OK)
    {
        wstring message = StringUtilities::Format(L"SQL error preparing statement. Error code: %d", result);
        throw SelectedTextTranslateException(message);
    }

    return statement;
}

void SqliteProvider::BindValue(sqlite3_stmt* statement, wstring name, wstring value) const
{
    BindValue(statement, name, [=, &statement](int index) -> int
    {
        return sqlite3_bind_text(statement, index, StringUtilities::GetUtf8String(value).c_str(), -1, SQLITE_TRANSIENT);
    });
}

void SqliteProvider::BindValue(sqlite3_stmt* statement, wstring name, int value) const
{
    BindValue(statement, name, [=, &statement](int index) -> int
    {
        return sqlite3_bind_int(statement, index, value);
    });
}

void SqliteProvider::BindValue(sqlite3_stmt* statement, wstring name, long long value) const
{
    BindValue(statement, name, [=, &statement](int index) -> int
    {
        return sqlite3_bind_int64(statement, index, value);
    });
}

void SqliteProvider::BindValue(sqlite3_stmt* statement, wstring name, function<int(int)> bindFunction) const
{
    int index = sqlite3_bind_parameter_index(statement, StringUtilities::GetUtf8String(name).c_str());
    int result = bindFunction(index);

    if (result != SQLITE_OK)
    {
        wstring message = StringUtilities::Format(L"SQL error binding value. Error code: %d", result);
        throw SelectedTextTranslateException(message);
    }
}

void SqliteProvider::ExecuteNonQuery(sqlite3_stmt *statement) const
{
    sqlite3_step(statement);
    int result = sqlite3_finalize(statement);

    if (result != SQLITE_OK)
    {
        wstring message = StringUtilities::Format(L"SQL error executing non query. Error code: %d", result);
        throw SelectedTextTranslateException(message);
    }
}

void SqliteProvider::ExecuteReader(sqlite3_stmt* statement, function<void(map<wstring, int>)> rowCallback) const
{
    int result = sqlite3_step(statement);
    while (result == SQLITE_ROW)
    {
        map<wstring, int> columnsIndexes;
        int columnsCount = sqlite3_column_count(statement);
        for(int i = 0; i < columnsCount; ++i)
        {
            wchar_t* columnName = (wchar_t*)sqlite3_column_name16(statement, i);
            columnsIndexes[wstring(columnName)] = i;
        }

        rowCallback(columnsIndexes);

        result = sqlite3_step(statement);
    }

    result = sqlite3_finalize(statement);

    if (result != SQLITE_OK)
    {
        wstring message = StringUtilities::Format(L"SQL error getting log records. Error code: %d", result);
        throw SelectedTextTranslateException(message);
    }
}

SqliteProvider::~SqliteProvider()
{
}
