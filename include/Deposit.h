#pragma once
#include <string>
#include <memory>

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

    double getScore() const { return score; }
    bool isEarlyWithdrawalPenalized() const { return hasEarlyWithdrawalPenalty; }
    int getBankRating() const { return bankRating; }

    void setScore(double newScore) { score = newScore; }
    void setBankRating(int rating) { bankRating = rating; }
};