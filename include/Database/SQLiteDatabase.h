#pragma once
#include "IDatabase.h"
#include <string>
#include <sqlite3.h>
#include <iostream>
#include <stdexcept>

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
    bool saveDepositScores(const std::vector<std::shared_ptr<Deposit>>& deposits) override;
    bool addDeposit(const Deposit& deposit, int bankId) override;
    std::vector<std::pair<int, std::string>> getAllBanks();
    bool addBank(const std::string& name, const std::string& license, double rating);
    bool deleteBank(int bankId) override;
    bool deleteDeposit(int depositId) override;
    virtual std::vector<Bank> getAllBanksDetailed() override;
};  