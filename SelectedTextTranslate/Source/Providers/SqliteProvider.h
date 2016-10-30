#pragma once
class SqliteProvider
{
private:
    void ApplyPragmaStatement(sqlite3* database, string statement) const;

public:
    SqliteProvider();
    ~SqliteProvider();

    sqlite3* OpenDatabase(wstring databaseName) const;
    sqlite3_stmt* CreateStatement(sqlite3* database, const char* sqlQuery) const;

    void BindValue(sqlite3_stmt* statement, wstring name, wstring value) const;
    void BindValue(sqlite3_stmt* statement, wstring name, int value) const;
    void BindValue(sqlite3_stmt* statement, wstring name, long long value) const;
    void BindValue(sqlite3_stmt* statement, wstring name, function<int(int)> bindFunction) const;

    void ExecuteNonQuery(sqlite3_stmt* statement) const;
    void ExecuteReader(sqlite3_stmt* statement, function<void(map<wstring, int>)> rowCallback) const;
};