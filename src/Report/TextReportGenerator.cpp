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

    out << "Рекомендуемые вклады\n";
    out << "================================\n";
    out << "Рейтинг рассчитан на основе ваших требований и предпочтений.\n";
    out << "Вклады отсортированы по степени соответствия вашим критериям.\n\n";

    for (const auto& deposit : recommended) {
        out << "Банк: " << deposit->getBankName() << "\n";
        out << "Название вклада: " << deposit->getName() << "\n";
        out << "Процентная ставка: " << deposit->getInterestRate() << "%\n";
        out << "Срок: " << deposit->getTermMonths() << " месяцев\n";
        out << "Минимальная сумма: " << deposit->getMinAmount() << " руб.\n";
        out << "Пополнение: " << (deposit->isReplenishable() ? "Да" : "Нет") << "\n";
        out << "Частичное снятие: " << (deposit->isWithdrawable() ? "Да" : "Нет") << "\n";
        out << "Капитализация: " << (deposit->hasCapitalization() ? "Да" : "Нет") << "\n";
        out << "Рейтинг: " << deposit->getScore() << "\n";
        out << "--------------------------------\n";
    }

    out << "\nОбщий рейтинг всех вкладов\n";
    out << "================================\n";
    out << "Рейтинг рассчитан на основе общей выгодности вкладов,\n";
    out << "независимо от ваших требований. Учитываются:\n";
    out << "- Процентная ставка\n";
    out << "- Наличие капитализации\n";
    out << "- Возможность пополнения и снятия\n";
    out << "- Рейтинг банка\n\n";

    std::vector<std::shared_ptr<Deposit>> sortedDeposits = allDeposits;

    for (auto& deposit : sortedDeposits) {
        deposit->setScore(calculateGeneralScore(deposit));
    }

    std::sort(sortedDeposits.begin(), sortedDeposits.end(),
        [](const auto& a, const auto& b) {
            return a->getScore() > b->getScore();
        });

    for (const auto& deposit : sortedDeposits) {
        out << "Банк: " << deposit->getBankName() << "\n";
        out << "Название вклада: " << deposit->getName() << "\n";
        out << "Процентная ставка: " << deposit->getInterestRate() << "%\n";
        out << "Срок: " << deposit->getTermMonths() << " месяцев\n";
        out << "Минимальная сумма: " << deposit->getMinAmount() << " руб.\n";
        out << "Пополнение: " << (deposit->isReplenishable() ? "Да" : "Нет") << "\n";
        out << "Частичное снятие: " << (deposit->isWithdrawable() ? "Да" : "Нет") << "\n";
        out << "Капитализация: " << (deposit->hasCapitalization() ? "Да" : "Нет") << "\n";
        out << "Общий рейтинг: " << deposit->getScore() << "\n";
        out << "--------------------------------\n";
    }
}

void TextReportGenerator::printToConsole(
    const std::vector<std::shared_ptr<Deposit>>& deposits) const {

    std::cout << "\nДетализация вкладов:\n";
    std::cout << "================================\n";

    for (const auto& deposit : deposits) {
        std::cout << "Банк: " << deposit->getBankName() << "\n";
        std::cout << "Название: " << deposit->getName() << "\n";
        std::cout << "Ставка: " << deposit->getInterestRate() << "%";
        std::cout << ", Срок: " << deposit->getTermMonths() << " мес.\n";
        std::cout << "Мин. сумма: " << deposit->getMinAmount() << " руб.\n";
        std::cout << "Пополнение: " << (deposit->isReplenishable() ? "Да" : "Нет") << ", ";
        std::cout << "Снятие: " << (deposit->isWithdrawable() ? "Да" : "Нет") << ", ";
        std::cout << "Капитализация: " << (deposit->hasCapitalization() ? "Да" : "Нет") << "\n";
        std::cout << "Рейтинг: " << deposit->getScore() << " баллов\n";
        std::cout << "--------------------------------\n";
    }
}