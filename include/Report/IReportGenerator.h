#pragma once
#include "Deposit.h"
#include <vector>
#include <memory>
#include <algorithm>

class IReportGenerator {
public:
    virtual ~IReportGenerator() = default;
    virtual void generateReport(
        const std::vector<std::shared_ptr<Deposit>>& recommended,
        const std::vector<std::shared_ptr<Deposit>>& allDeposits,
        const std::string& filename = "") = 0;
};
