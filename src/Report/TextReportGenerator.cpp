#include "TextReportGenerator.h"
#pragma execution_character_set("utf-8")

void TextReportGenerator::generateReport(
    const std::vector<std::shared_ptr<Deposit>>& recommended,
    const std::vector<std::shared_ptr<Deposit>>& allDeposits,
    const std::string& filename) {

    if (filename.empty()) {
        std::cout << "\n����������� �������:\n";
        std::cout << "================================\n";

        for (const auto& deposit : recommended) {
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
    } else {
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
        BasicDepositAnalyzer analyzer;

        for (auto& deposit : sortedDeposits) {
            deposit->setScore(analyzer.calculateDepositScore(*deposit));
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
}