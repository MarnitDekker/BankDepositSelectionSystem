#include "Client.h"

Client::Client(double amount, int term, bool needReplenishable, bool needWithdrawable)
    : amount(amount), termMonths(term),
    needReplenishable(needReplenishable),
    needWithdrawable(needWithdrawable) {
}

double Client::getAmount() const { return amount; }
int Client::getTerm() const { return termMonths; }
bool Client::needsReplenishable() const { return needReplenishable; }
bool Client::needsWithdrawable() const { return needWithdrawable; }