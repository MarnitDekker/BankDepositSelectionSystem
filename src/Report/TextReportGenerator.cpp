#include "TextReportGenerator.h"
#pragma execution_character_set("utf-8")

double calculateGeneralScore(const std::shared_ptr<Deposit>& deposit) {
    double score = 0.0;

    score += deposit->getInterestRate() * 10;

    if (deposit->hasCapitalization()) score += 5;
    if (deposit->isReplenishable()) score += 3;
    if (deposit->isWithdrawable()) score += 2;

    if (deposit->isEarlyWithdrawalPenalized()) score -= 5;

    score += deposit->getBankRating() * 2;

    return score;
}

void TextReportGenerator::generateReport(
    const std::vector<std::shared_ptr<Deposit>>& recommended,
    const std::vector<std::shared_ptr<Deposit>>& allDeposits,
    const std::string& filename) {

    std::ofstream out(filename);
    if (!out) return;

    out << "������������� ������\n";
    out << "================================\n";
    out << "������� ��������� �� ������ ����� ���������� � ������������.\n";
    out << "������ ������������� �� ������� ������������ ����� ���������.\n\n";

    for (const auto& deposit : recommended) {
        out << "����: " << deposit->getBankName() << "\n";
        out << "�������� ������: " << deposit->getName() << "\n";
        out << "���������� ������: " << deposit->getInterestRate() << "%\n";
        out << "����: " << deposit->getTermMonths() << " �������\n";
        out << "����������� �����: " << deposit->getMinAmount() << " ���.\n";
        out << "����������: " << (deposit->isReplenishable() ? "��" : "���") << "\n";
        out << "��������� ������: " << (deposit->isWithdrawable() ? "��" : "���") << "\n";
        out << "�������������: " << (deposit->hasCapitalization() ? "��" : "���") << "\n";
        out << "�������: " << deposit->getScore() << "\n";
        out << "--------------------------------\n";
    }

    out << "\n����� ������� ���� �������\n";
    out << "================================\n";
    out << "������� ��������� �� ������ ����� ���������� �������,\n";
    out << "���������� �� ����� ����������. �����������:\n";
    out << "- ���������� ������\n";
    out << "- ������� �������������\n";
    out << "- ����������� ���������� � ������\n";
    out << "- ������� �����\n\n";

    std::vector<std::shared_ptr<Deposit>> sortedDeposits = allDeposits;

    for (auto& deposit : sortedDeposits) {
        deposit->setScore(calculateGeneralScore(deposit));
    }

    std::sort(sortedDeposits.begin(), sortedDeposits.end(),
        [](const auto& a, const auto& b) {
            return a->getScore() > b->getScore();
        });

    for (const auto& deposit : sortedDeposits) {
        out << "����: " << deposit->getBankName() << "\n";
        out << "�������� ������: " << deposit->getName() << "\n";
        out << "���������� ������: " << deposit->getInterestRate() << "%\n";
        out << "����: " << deposit->getTermMonths() << " �������\n";
        out << "����������� �����: " << deposit->getMinAmount() << " ���.\n";
        out << "����������: " << (deposit->isReplenishable() ? "��" : "���") << "\n";
        out << "��������� ������: " << (deposit->isWithdrawable() ? "��" : "���") << "\n";
        out << "�������������: " << (deposit->hasCapitalization() ? "��" : "���") << "\n";
        out << "����� �������: " << deposit->getScore() << "\n";
        out << "--------------------------------\n";
    }
}

void TextReportGenerator::printToConsole(
    const std::vector<std::shared_ptr<Deposit>>& deposits) const {

    std::cout << "\n����������� �������:\n";
    std::cout << "================================\n";

    for (const auto& deposit : deposits) {
        std::cout << "����: " << deposit->getBankName() << "\n";
        std::cout << "��������: " << deposit->getName() << "\n";
        std::cout << "������: " << deposit->getInterestRate() << "%";
        std::cout << ", ����: " << deposit->getTermMonths() << " ���.\n";
        std::cout << "���. �����: " << deposit->getMinAmount() << " ���.\n";
        std::cout << "����������: " << (deposit->isReplenishable() ? "��" : "���") << ", ";
        std::cout << "������: " << (deposit->isWithdrawable() ? "��" : "���") << ", ";
        std::cout << "�������������: " << (deposit->hasCapitalization() ? "��" : "���") << "\n";
        std::cout << "�������: " << deposit->getScore() << " ������\n";
        std::cout << "--------------------------------\n";
    }
}