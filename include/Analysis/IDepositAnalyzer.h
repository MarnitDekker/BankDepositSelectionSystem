#pragma once
#include <vector>
#include <memory>
#include "Deposit.h"
#include "Client.h"

class IDepositAnalyzer {
public:
    virtual ~IDepositAnalyzer() = default;
    virtual std::vector<std::shared_ptr<Deposit>> analyze(
        const std::vector<std::shared_ptr<Deposit>>& deposits,
        const Client& client) = 0;
};