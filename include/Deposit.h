#pragma once
#include <string>

class Deposit {
private:
    int id;
    std::string name;
    double interestRate;
    int termMonths;
    double minAmount;
    bool replenishable;
    bool withdrawable;
    bool capitalization;
    std::string bankName;
    double score;
    bool hasEarlyWithdrawalPenalty;
    int bankRating;

public:
    Deposit(int id, const std::string& name, double rate, int term,
        double minAmount, bool replenishable, bool withdrawable,
        bool capitalization, const std::string& bankName,
        bool earlyWithdrawal = false);
};