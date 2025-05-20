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

    int getId() const;
    std::string getName() const;
    double getInterestRate() const;
    int getTermMonths() const;
    double getMinAmount() const;
    bool isReplenishable() const;
    bool isWithdrawable() const;
    bool hasCapitalization() const;
    std::string getBankName() const;

    double getScore() const;
    bool isEarlyWithdrawalPenalized() const;
    int getBankRating() const;

    void setScore(double newScore);
    void setBankRating(int rating);

    double calculateIncome(double amount) const;
};