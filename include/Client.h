#pragma once
#include <string>

class Client {
private:
    double amount;
    int termMonths;
    bool needReplenishable;
    bool needWithdrawable;
public:
    Client(double amount, int term, bool needReplenishable, bool needWithdrawable);
};