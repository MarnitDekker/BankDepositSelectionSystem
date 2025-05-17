#pragma once
#include "IDatabase.h"
#include "BasicDepositAnalyzer.h"
#include "IDepositAnalyzer.h"
#include "IRecommendationStrategy.h"
#include "IReportGenerator.h"
#include "TextReportGenerator.h"
#include "Client.h"

class AppController {
private:
    std::unique_ptr<IDatabase> database;
    std::unique_ptr<IDepositAnalyzer> analyzer;
    /*std::unique_ptr<IRecommendationStrategy> recommendationStrategy;
    std::unique_ptr<IReportGenerator> reportGenerator;
    std::unique_ptr<IReportGenerator> consoleReportGenerator;*/

    void printTopDeposits(const std::vector<std::shared_ptr<Deposit>>& deposits,
        const Client& client, size_t count) const;
    /*void generateFullReport(const std::vector<std::shared_ptr<Deposit>>& deposits,
        const Client& client);*/

public:
    AppController(std::unique_ptr<IDatabase> db,
        std::unique_ptr<IDepositAnalyzer> analyzer);

    void processClientRequest(const Client& client);

    /*void setRecommendationStrategy(std::unique_ptr<IRecommendationStrategy> strategy);*/
    /*void setReportGenerator(std::unique_ptr<IReportGenerator> reporter);
    void setConsoleReportGenerator(std::unique_ptr<IReportGenerator> reporter);*/

    void showAllDeposits() const;
    void logUserQuery(const Client& client) const;
    std::vector<std::shared_ptr<Deposit>> getAllDeposits() const;

    bool addNewDeposit(const Deposit& deposit, int bankId);

    std::vector<std::pair<int, std::string>> getAllBanks();
};