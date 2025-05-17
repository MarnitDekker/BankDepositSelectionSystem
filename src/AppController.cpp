#include "../AppController.h"
#include "Factory.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#pragma execution_character_set("utf-8")

AppController::AppController(std::unique_ptr<IDatabase> db,
    std::unique_ptr<IDepositAnalyzer> analyzer)
    : database(std::move(db)), analyzer(std::move(analyzer)) {
}

void AppController::processClientRequest(const Client& client) {
    auto allDeposits = database->getAllDeposits();
    if (allDeposits.empty()) {
        std::cout << "В системе нет доступных вкладов.\n";
        return;
    }

    auto suitableDeposits = analyzer->analyze(allDeposits, client);

    if (!database->saveDepositScores(suitableDeposits)) {
        std::cerr << "Предупреждение: не удалось сохранить баллы в БД.\n";
    }

    auto recommendationGen = Factory::createStrategy(
        (client.getTerm() == 0) ? Factory::StrategyType::TOP_RATE : Factory::StrategyType::FLEXIBLE_TERM
    );

    if (recommendationGen) {
        suitableDeposits = recommendationGen->recommend(suitableDeposits, client);
    }

    if (suitableDeposits.empty()) {
        std::cout << "\nПо вашим критериям не найдено подходящих вкладов.\n";
        return;
    }

    printTopDeposits(suitableDeposits, client, 3);

    std::cout << "\nХотите получить подробный отчет? (1 - Да, 0 - Нет): ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "\nВыберите формат отчета:\n";
        std::cout << "1 - Текстовый отчет в консоли\n";
        std::cout << "0 - Подробный HTML отчет в браузере\n";
        std::cout << "Ваш выбор: ";
        int reportChoice;
        std::cin >> reportChoice;

        auto reportGen = Factory::createReportGenerator(
            reportChoice == 1 ? Factory::ReportType::TEXT : Factory::ReportType::HTML
        );

        if (reportGen) {
            if (reportChoice == 1) {
                if (auto textGen = dynamic_cast<TextReportGenerator*>(reportGen.get())) {
                    textGen->printToConsole(suitableDeposits);
                }
            } else {
                reportGen->generateReport(suitableDeposits, database->getAllDeposits(), "deposit_report.html");
            }
        }
    }

    std::cout << "\nСпасибо за использование нашей системы!\n";
}

void AppController::printTopDeposits(
    const std::vector<std::shared_ptr<Deposit>>& deposits,
    const Client& client, size_t count) const {

    std::cout << "\nТоп-" << count << " рекомендованных вкладов:\n";
    for (size_t i = 0; i < std::min(count, deposits.size()); ++i) {
        const auto& deposit = deposits[i];
        std::cout << i + 1 << ". " << deposit->getBankName()
            << " - " << deposit->getName() << "\n";
        std::cout << "   Ставка: " << deposit->getInterestRate() << "%";
        std::cout << ", Срок: " << deposit->getTermMonths() << " мес.\n";
        std::cout << "   Мин. сумма: " << deposit->getMinAmount() << " руб.";
        std::cout << ", Доход: " << deposit->calculateIncome(client.getAmount()) << " руб.\n";
        std::cout << "   Рейтинг: " << deposit->getScore() << " баллов\n\n";
    }
}

void AppController::logUserQuery(const Client& client) const {
    std::ofstream out("user_queries.log", std::ios::app);
    if (!out) {
        std::cerr << "Ошибка открытия файла лога\n";
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    char timestamp[32];
    std::tm tm_buf;
    localtime_s(&tm_buf, &now_time_t);
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_buf);

    out << "[" << timestamp << "] "
        << "Сумма: " << client.getAmount() << " руб., "
        << "Срок: " << client.getTerm() << " мес., "
        << "Пополнение: " << (client.needsReplenishable() ? "Да" : "Нет") << ", "
        << "Снятие: " << (client.needsWithdrawable() ? "Да" : "Нет") << "\n";
}

std::vector<std::shared_ptr<Deposit>> AppController::getAllDeposits() const {
    return database->getAllDeposits();
}

void AppController::showAllDeposits() const {
    auto deposits = database->getAllDeposits();
    if (deposits.empty()) {
        std::cout << "В системе нет доступных вкладов.\n";
        return;
    }

    std::cout << "\nСписок всех доступных вкладов:\n";
    for (const auto& deposit : deposits) {
        std::cout << deposit->getId() << ". " << deposit->getBankName()
            << " - " << deposit->getName() << "\n";
        std::cout << "   Ставка: " << deposit->getInterestRate() << "%";
        std::cout << ", Срок: " << deposit->getTermMonths() << " мес.\n";
        std::cout << "   Мин. сумма: " << deposit->getMinAmount() << " руб.\n";
    }
}

bool AppController::addNewDeposit(const Deposit& deposit, int bankId) {
    return database->addDeposit(deposit, bankId);
}

std::vector<std::pair<int, std::string>> AppController::getAllBanks() {
    return database->getAllBanks();
}

bool AppController::deleteDeposit(int depositId) {
    std::string sql = "DELETE FROM deposits WHERE id = " + std::to_string(depositId) + ";";
    return database->executeSQL(sql);
}