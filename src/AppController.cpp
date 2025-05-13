#include "AppController.h"
#include "BasicDepositAnalyzer.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#pragma execution_character_set("utf-8")

AppController::AppController(std::unique_ptr<IDatabase> db,
    std::unique_ptr<IDepositAnalyzer> analyzer)
    : database(std::move(db)), analyzer(std::move(analyzer)) {
}

void AppController::logUserQuery(const Client& client) const {
    std::ofstream out("user_queries.log", std::ios::app);
    if (!out) {
        std::cerr << "������ �������� ����� ����\n";
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    char timestamp[32];
    std::tm tm_buf;
    localtime_s(&tm_buf, &now_time_t);
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm_buf);

    out << "[" << timestamp << "] "
        << "�����: " << client.getAmount() << " ���., "
        << "����: " << client.getTerm() << " ���., "
        << "����������: " << (client.needsReplenishable() ? "��" : "���") << ", "
        << "������: " << (client.needsWithdrawable() ? "��" : "���") << "\n";
}

std::vector<std::shared_ptr<Deposit>> AppController::getAllDeposits() const {
    return database->getAllDeposits();
}

void AppController::showAllDeposits() const {
    auto deposits = database->getAllDeposits();
    if (deposits.empty()) {
        std::cout << "� ������� ��� ��������� �������.\n";
        return;
    }

    std::cout << "\n������ ���� ��������� �������:\n";
    for (const auto& deposit : deposits) {
        std::cout << deposit->getId() << ". " << deposit->getBankName()
            << " - " << deposit->getName() << "\n";
        std::cout << "   ������: " << deposit->getInterestRate() << "%";
        std::cout << ", ����: " << deposit->getTermMonths() << " ���.\n";
        std::cout << "   ���. �����: " << deposit->getMinAmount() << " ���.\n";
    }
}