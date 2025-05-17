#include "../AppController.h"
#include "Factory.h"
//#include <iostream>
//#include <fstream>
//#include <chrono>
//#include <windows.h>
//#include <limits> 
//#include <filesystem>
//#include <algorithm>
#pragma execution_character_set("utf-8")

AppController::AppController(std::unique_ptr<IDatabase> db,
    std::unique_ptr<IDepositAnalyzer> analyzer)
    : database(std::move(db)), analyzer(std::move(analyzer)) {
}

void AppController::processClientRequest(const Client& client) {
    auto allDeposits = database->getAllDeposits();
    if (allDeposits.empty()) {
        std::cout << "� ������� ��� ��������� �������.\n";
        return;
    }

    auto suitableDeposits = analyzer->analyze(allDeposits, client);

    if (!database->saveDepositScores(suitableDeposits)) {
        std::cerr << "��������������: �� ������� ��������� ����� � ��.\n";
    }

    auto recommendationGen = Factory::createStrategy(
        (client.getTerm() == 0) ? Factory::StrategyType::TOP_RATE : Factory::StrategyType::FLEXIBLE_TERM
    );

    if (recommendationGen) {
        suitableDeposits = recommendationGen->recommend(suitableDeposits, client);
    }

    if (suitableDeposits.empty()) {
        std::cout << "\n�� ����� ��������� �� ������� ���������� �������.\n";
        return;
    }

    printTopDeposits(suitableDeposits, client, 3);

    std::cout << "\n������ �������� ��������� �����? (1 - ��, 0 - ���): ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        std::cout << "\n�������� ������ ������:\n";
        std::cout << "1 - ��������� ����� � �������\n";
        std::cout << "0 - ��������� HTML ����� � ��������\n";
        std::cout << "��� �����: ";
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

    std::cout << "\n������� �� ������������� ����� �������!\n";
}

void AppController::printTopDeposits(
    const std::vector<std::shared_ptr<Deposit>>& deposits,
    const Client& client, size_t count) const {

    std::cout << "\n���-" << count << " ��������������� �������:\n";
    for (size_t i = 0; i < (std::min)(count, deposits.size()); ++i) {
        const auto& deposit = deposits[i];
        std::cout << i + 1 << ". " << deposit->getBankName()
            << " - " << deposit->getName() << "\n";
        std::cout << "   ������: " << deposit->getInterestRate() << "%";
        std::cout << ", ����: " << deposit->getTermMonths() << " ���.\n";
        std::cout << "   ���. �����: " << deposit->getMinAmount() << " ���.";
        std::cout << ", �����: " << deposit->calculateIncome(client.getAmount()) << " ���.\n";
        std::cout << "   �������: " << deposit->getScore() << " ������\n\n";
    }
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

void AppController::showUserQueryHistory() const {
    std::ifstream in("user_queries.log");
    if (!in) {
        std::cout << "������� �������� �����.\n";
        return;
    }

    std::cout << "\n������� �������� �������������:\n";
    std::cout << "----------------------------------------\n";
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << '\n';
    }
    std::cout << "----------------------------------------\n";
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

//bool AppController::addNewDeposit(const Deposit& deposit, int bankId) {
//    return database->addDeposit(deposit, bankId);
//}

std::vector<std::pair<int, std::string>> AppController::getAllBanks() {
    return database->getAllBanks();
}

bool AppController::deleteDeposit(int depositId) {
    std::string sql = "DELETE FROM deposits WHERE id = " + std::to_string(depositId) + ";";
    return database->executeSQL(sql);
}

std::string AppController::cp1251_to_utf8(const std::string& cp1251str) const {
    int wchars_num = MultiByteToWideChar(1251, 0, cp1251str.c_str(), -1, NULL, 0);
    std::wstring wstr(wchars_num, 0);
    MultiByteToWideChar(1251, 0, cp1251str.c_str(), -1, &wstr[0], wchars_num);

    int utf8_num = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string utf8str(utf8_num, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8str[0], utf8_num, NULL, NULL);

    if (!utf8str.empty() && utf8str.back() == '\0') utf8str.pop_back();
    return utf8str;
}

void AppController::handleAddDeposit() {
    std::cout << "������ ������:\n";
    auto banks = database->getAllBanks();
    std::vector<std::pair<int, std::string>> nonEmptyBanks;
    int idx = 1;

    for (const auto& bank : banks) {
        if (!bank.second.empty()) {
            std::cout << idx << ". " << bank.second << std::endl;
            nonEmptyBanks.push_back(bank);
            ++idx;
        }
    }

    if (nonEmptyBanks.empty()) {
        std::cout << "��� ��������� ������!\n";
        return;
    }

    int bankChoice = 0;
    std::cout << "�������� ����� ����� �� ������: ";
    std::cin >> bankChoice;

    if (bankChoice < 1 || bankChoice >(int)nonEmptyBanks.size()) {
        std::cout << "������������ ����� �����!\n";
        return;
    }

    int bankId = nonEmptyBanks[bankChoice - 1].first;
    std::string bankName = nonEmptyBanks[bankChoice - 1].second;
    std::cout << "������� ��������� ������ ������:\n";

    int id = 0;
    double rate, minAmount;
    int term;
    bool replenishable, withdrawable, capitalization, earlyWithdrawal;

    std::filesystem::path path = std::filesystem::current_path();
    for (int i = 0; i < 5; ++i) {
        if (std::filesystem::exists(path / "CMakeLists.txt")) {
            break;
        }
        path = path.parent_path();
    }

    std::filesystem::path dirPath = path / "data";
    std::filesystem::path filePath = dirPath / "nameDeposit.txt";

    std::string tempName;
    std::cout << "������� �������� ������: ";
    std::cin.ignore();
    std::getline(std::cin, tempName);

    std::string utf8Name = cp1251_to_utf8(tempName);

    std::ofstream fout(filePath.string());
    if (fout.is_open()) {
        fout << utf8Name;
        fout.close();
    }
    else {
        std::cout << "������ ��� �������� ����� " << filePath << "!" << std::endl;
    }

    std::string name;
    {
        std::ifstream fin(filePath.string());
        if (fin.is_open()) {
            std::getline(fin, name);
            fin.close();
        }
        else {
            std::cout << "������ �������� ����� " << filePath << "!" << std::endl;
        }
    }

    std::cout << "������ (%): "; std::cin >> rate;
    std::cout << "���� (���): "; std::cin >> term;
    std::cout << "���. �����: "; std::cin >> minAmount;
    std::cout << "���������� (1 - ��, 0 - ���): "; std::cin >> replenishable;
    std::cout << "������ (1 - ��, 0 - ���): "; std::cin >> withdrawable;
    std::cout << "������������� (1 - ��, 0 - ���): "; std::cin >> capitalization;
    std::cout << "����� �� ��������� ������ (1 - ��, 0 - ���): "; std::cin >> earlyWithdrawal;

    Deposit dep(id, name, rate, term, minAmount, replenishable, withdrawable, capitalization, bankName, earlyWithdrawal);
    if (database->addDeposit(dep, bankId)) {
        std::cout << "����� ������� ��������!\n";
    }
    else {
        std::cout << "������ ��� ���������� ������.\n";
    }

    std::remove(filePath.string().c_str());
}

void AppController::handleAddBank() {
    std::string name, license;
    double rating;

    std::filesystem::path path = std::filesystem::current_path();
    for (int i = 0; i < 5; ++i) {
        if (std::filesystem::exists(path / "CMakeLists.txt")) {
            break;
        }
        path = path.parent_path();
    }

    std::filesystem::path dirPath = path / "data";
    std::filesystem::path filePath = dirPath / "nameBank.txt";

    std::string tempName;
    std::cout << "������� �������� �����: ";
    std::cin.ignore();
    std::getline(std::cin, tempName);

    std::string utf8Name = cp1251_to_utf8(tempName);

    std::ofstream fout(filePath.string());
    if (fout.is_open()) {
        fout << utf8Name;
        fout.close();
    }
    else {
        std::cout << "������ ��� �������� ����� " << filePath << "!" << std::endl;
    }

    {
        std::ifstream fin(filePath.string());
        if (fin.is_open()) {
            std::getline(fin, name);
            fin.close();
        }
        else {
            std::cout << "������ �������� ����� " << filePath << "!" << std::endl;
        }
    }
    std::cout << "������� ����� ��������: ";
    std::getline(std::cin, license);
    std::cout << "������� ������� ����� (0-5): ";
    std::cin >> rating;
    std::cin.ignore();

    if (database->addBank(name, license, rating)) {
        std::cout << "���� ������� ��������!\n";
    }
    else {
        std::cout << "������ ��� ���������� �����.\n";
    }
    std::remove(filePath.string().c_str());
}