#pragma once
#include <vector>
#include <memory>
#include "Deposit.h"
#include "Client.h"
#include "IDepositAnalyzer.h"

class BasicDepositAnalyzer : public IDepositAnalyzer {
public:
    virtual ~BasicDepositAnalyzer() = default;

    std::vector<std::shared_ptr<Deposit>> analyze(
        const std::vector<std::shared_ptr<Deposit>>& deposits,
        const Client& client) override;

protected:
    virtual double calculateDepositScore(
        const Deposit& deposit, const Client& client) const;
};