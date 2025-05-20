#include "HTMLReportGenerator.h"
//#include <fstream>
//#include <iomanip>
//#include <windows.h>
#pragma execution_character_set("utf-8")

static double calculateGeneralScore(const std::shared_ptr<Deposit>& deposit) {
    double score = 0.0;

    score += deposit->getInterestRate() * 10;

    if (deposit->hasCapitalization()) score += 5;
    if (deposit->isReplenishable()) score += 3;
    if (deposit->isWithdrawable()) score += 2;

    if (deposit->isEarlyWithdrawalPenalized()) score -= 5;

    score += deposit->getBankRating() * 2;

    return score;
}

void HTMLReportGenerator::generateReport(
    const std::vector<std::shared_ptr<Deposit>>& recommended,
    const std::vector<std::shared_ptr<Deposit>>& allDeposits,
    const std::string& filename) {

    std::ofstream out(filename, std::ios::out | std::ios::trunc);
    if (!out) return;

    out << "<!DOCTYPE html>\n<html>\n<head>\n"
        << "<meta charset=\"UTF-8\">\n"
        << "<title>������������� ������</title>\n"
        << "<style>\n"
        << "table { border-collapse: collapse; width: 100%; margin-bottom: 20px; }\n"
        << "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n"
        << "th { background-color: #f2f2f2; }\n"
        << "tr:nth-child(even) { background-color: #f9f9f9; }\n"
        << ".explanation { margin: 20px 0; padding: 10px; background-color: #f8f9fa; border-left: 4px solid #007bff; }\n"
        << "</style>\n</head>\n<body>\n"
        << "<h1>������������� ������</h1>\n"
        << "<div class=\"explanation\">\n"
        << "<p><strong>������� � ���� �������</strong> ��������� �� ������ ����� ���������� � ������������. "
        << "������ ������������� �� ������� ������������ ����� ���������.</p>\n"
        << "</div>\n"
        << "<table>\n"
        << "<tr><th>����</th><th>��������</th><th>������</th>"
        << "<th>����</th><th>���. �����</th><th>����������</th>"
        << "<th>������</th><th>�������������</th><th>�������</th></tr>\n";

    for (const auto& deposit : recommended) {
        out << "<tr>"
            << "<td>" << deposit->getBankName() << "</td>"
            << "<td>" << deposit->getName() << "</td>"
            << "<td>" << deposit->getInterestRate() << "%</td>"
            << "<td>" << deposit->getTermMonths() << " ���.</td>"
            << "<td>" << std::to_string(static_cast<long long>(deposit->getMinAmount())) << " ���.</td>"
            << "<td>" << (deposit->isReplenishable() ? "��" : "���") << "</td>"
            << "<td>" << (deposit->isWithdrawable() ? "��" : "���") << "</td>"
            << "<td>" << (deposit->hasCapitalization() ? "��" : "���") << "</td>"
            << "<td>" << deposit->getScore() << "</td>"
            << "</tr>\n";
    }
    out << "</table>\n";

    out << "<h2>����� ������� ���� �������</h2>\n"
        << "<div class=\"explanation\">\n"
        << "<p><strong>������� � ���� �������</strong> ��������� �� ������ ����� ���������� �������, "
        << "���������� �� ����� ����������. �����������: ���������� ������, ������� �������������, "
        << "����������� ���������� � ������, ������� �����.</p>\n"
        << "</div>\n"
        << "<table>\n<tr><th>����</th><th>��������</th><th>������</th><th>����</th>"
        << "<th>���. �����</th><th>����������</th><th>������</th><th>�������������</th><th>����� �������</th></tr>\n";


    std::vector<std::shared_ptr<Deposit>> sortedDeposits = allDeposits;

    for (auto& deposit : sortedDeposits) {
        deposit->setScore(calculateGeneralScore(deposit));
    }

    std::sort(sortedDeposits.begin(), sortedDeposits.end(),
        [](const auto& a, const auto& b) {
            return a->getScore() > b->getScore();
        });

    for (const auto& deposit : sortedDeposits) {
        out << "<tr>"
            << "<td>" << deposit->getBankName() << "</td>"
            << "<td>" << deposit->getName() << "</td>"
            << "<td>" << deposit->getInterestRate() << "%</td>"
            << "<td>" << deposit->getTermMonths() << " ���.</td>"
            << "<td>" << std::to_string(static_cast<long long>(deposit->getMinAmount())) << " ���.</td>"
            << "<td>" << (deposit->isReplenishable() ? "��" : "���") << "</td>"
            << "<td>" << (deposit->isWithdrawable() ? "��" : "���") << "</td>"
            << "<td>" << (deposit->hasCapitalization() ? "��" : "���") << "</td>"
            << "<td>" << deposit->getScore() << "</td>"
            << "</tr>\n";
    }
    out << "</table>\n";
    out << "</body>\n</html>";

    ShellExecuteA(0, "open", filename.c_str(), 0, 0, SW_SHOWNORMAL);
}