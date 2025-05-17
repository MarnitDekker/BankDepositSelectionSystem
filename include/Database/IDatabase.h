#pragma once
#include <vector>
#include <memory>
#include "Deposit.h"

class IDatabase {
public:
    virtual ~IDatabase() = default;
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool executeSQL(const std::string& sql) = 0;
    virtual bool initializeDatabase() = 0;
    virtual std::vector<std::shared_ptr<Deposit>> getAllDeposits() = 0;
    virtual bool saveDepositScores(const std::vector<std::shared_ptr<Deposit>>& deposits) = 0;
    virtual bool addDeposit(const Deposit& deposit, int bankId) = 0;
    virtual std::vector<std::pair<int, std::string>> getAllBanks() = 0;
    virtual bool addBank(const std::string& name, const std::string& license, double rating) = 0;
};