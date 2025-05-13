#include "BasicDepositAnalyzer.h"
#include <algorithm>
#include <iostream>
#include <cmath>

std::vector<std::shared_ptr<Deposit>> BasicDepositAnalyzer::analyze(
    const std::vector<std::shared_ptr<Deposit>>& deposits,
    const Client& client) {

    std::vector<std::shared_ptr<Deposit>> suitableDeposits;
    std::vector<std::shared_ptr<Deposit>> partialDeposits;

    for (const auto& deposit : deposits) {
        if (deposit->getMinAmount() > client.getAmount()) continue;
        if (client.getTerm() != 0 && deposit->getTermMonths() != client.getTerm()) continue;

        bool replenishableOk = !client.needsReplenishable() || deposit->isReplenishable();
        bool withdrawableOk = !client.needsWithdrawable() || deposit->isWithdrawable();

        if (replenishableOk && withdrawableOk) {
            double score = calculateDepositScore(*deposit, client);
            deposit->setScore(score);
            suitableDeposits.push_back(deposit);
        } else if (replenishableOk || withdrawableOk) {
            double score = calculateDepositScore(*deposit, client) - 10;
            deposit->setScore(score);
            partialDeposits.push_back(deposit);
        }
    }

    if (suitableDeposits.empty() && !partialDeposits.empty()) {
        std::cout << "\nНет вкладов, одновременно поддерживающих пополнение и частичное снятие. Показаны вклады, удовлетворяющие хотя бы одному из условий.\n";
        suitableDeposits = partialDeposits;
    }

    std::sort(suitableDeposits.begin(), suitableDeposits.end(),
        [](const auto& a, const auto& b) {
            return a->getScore() > b->getScore();
        });

    return suitableDeposits;
}

double BasicDepositAnalyzer::calculateDepositScore(
    const Deposit& deposit, const Client& client) const {

    double score = 0.0;

    score += deposit.getInterestRate() * 10;
    score += deposit.getBankRating() * 2;

    if (deposit.hasCapitalization()) score += 5;
    if (deposit.isReplenishable()) score += 3;
    if (deposit.isWithdrawable()) score += 2;

    if (deposit.isEarlyWithdrawalPenalized()) score -= 5;

    return score;
}