#pragma once
#include <string>
#include <sqlite3.h>

class SQLiteDatabase {
private:
    std::string dbPath;
    sqlite3* db;
    bool executeSQL(const std::string& sql);
public:
    explicit SQLiteDatabase(const std::string& dbPath);
    ~SQLiteDatabase();

    bool connect();
    void disconnect();
};