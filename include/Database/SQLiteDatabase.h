#pragma once
#include <string>
#include <sqlite3.h>
#include <memory>
#include "IDatabase.h"

class SQLiteDatabase : public IDatabase {
private:
    std::string dbPath;
    sqlite3* db;
    bool executeSQL(const std::string& sql);
    void insertSampleData();
public:
    explicit SQLiteDatabase(const std::string& dbPath);
    ~SQLiteDatabase() override;

    bool connect() override;
    void disconnect() override;
    bool initializeDatabase() override;
    std::vector<std::shared_ptr<Deposit>> getAllDeposits() override;
};  