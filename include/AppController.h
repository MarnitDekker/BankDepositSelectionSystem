#pragma once
#include "IDatabase.h"
#include "BasicDepositAnalyzer.h"
#include "IDepositAnalyzer.h"
#include "IRecommendationStrategy.h"
#include "IReportGenerator.h"
#include "TextReportGenerator.h"
#include "Client.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <limits> 
#include <filesystem>
#include <algorithm>

class AppController {
private:
    std::unique_ptr<IDatabase> database;
    std::unique_ptr<IDepositAnalyzer> analyzer;

    void printTopDeposits(const std::vector<std::shared_ptr<Deposit>>& deposits,
        const Client& client, size_t count) const;

    std::string cp1251_to_utf8(const std::string& cp1251str) const;

public:
    AppController(std::unique_ptr<IDatabase> db,
        std::unique_ptr<IDepositAnalyzer> analyzer);

    void processClientRequest(const Client& client);

    void showAllDeposits() const;
    void logUserQuery(const Client& client) const;
    std::vector<std::shared_ptr<Deposit>> getAllDeposits() const;

    bool addNewDeposit(const Deposit& deposit, int bankId);
    bool deleteDeposit(int depositId);

    std::vector<std::pair<int, std::string>> getAllBanks();

    void showUserQueryHistory() const;
    void handleAddDeposit();
};