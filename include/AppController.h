#pragma once
#include "IDatabase.h"
#include "IDepositAnalyzer.h"
#include "Client.h"

class AppController {
private:
    std::unique_ptr<IDatabase> database;
    std::unique_ptr<IDepositAnalyzer> analyzer;

public:
    AppController(std::unique_ptr<IDatabase> db,
        std::unique_ptr<IDepositAnalyzer> analyzer);

    //void showAllDeposits() const;
    void logUserQuery(const Client& client) const;
/*    std::vector<std::shared_ptr<Deposit>> getAllDeposits() const*/;
};