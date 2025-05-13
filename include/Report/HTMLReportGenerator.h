#pragma once
#include "IReportGenerator.h"

class HTMLReportGenerator : public IReportGenerator {
public:
    void generateReport(
        const std::vector<std::shared_ptr<Deposit>>& recommended,
        const std::vector<std::shared_ptr<Deposit>>& allDeposits,
        const std::string& filename) override;
};