#include "Deposit.h"
#include <cmath>

Deposit::Deposit(int id, const std::string& name, double rate, int term,
    double minAmount, bool replenishable, bool withdrawable,
    bool capitalization, const std::string& bankName,
    bool earlyWithdrawal)
    : id(id), name(name), interestRate(rate), termMonths(term),
    minAmount(minAmount), replenishable(replenishable),
    withdrawable(withdrawable), capitalization(capitalization),
    bankName(bankName), hasEarlyWithdrawalPenalty(earlyWithdrawal),
    score(0.0), bankRating(0) {
}

int Deposit::getId() const { return id; }
std::string Deposit::getName() const { return name; }
double Deposit::getInterestRate() const { return interestRate; }
int Deposit::getTermMonths() const { return termMonths; }
double Deposit::getMinAmount() const { return minAmount; }
bool Deposit::isReplenishable() const { return replenishable; }
bool Deposit::isWithdrawable() const { return withdrawable; }
bool Deposit::hasCapitalization() const { return capitalization; }
std::string Deposit::getBankName() const { return bankName; }