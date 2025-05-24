#pragma once
#include "IDepositAnalyzer.h"
#include <algorithm>
#include <iostream>

class BasicDepositAnalyzer : public IDepositAnalyzer {
public:
    ~BasicDepositAnalyzer() override = default;

    std::vector<std::shared_ptr<Deposit>> analyze(
        const std::vector<std::shared_ptr<Deposit>>& deposits,
        const Client& client) override;

    double calculateDepositScore(
        const Deposit& deposit) const override;
};