#pragma once
#include "IReportGenerator.h"
#include "BasicDepositAnalyzer.h"
#include <iostream>
#include <fstream>

class TextReportGenerator : public IReportGenerator {
public:
    void generateReport(
        const std::vector<std::shared_ptr<Deposit>>& recommended,
        const std::vector<std::shared_ptr<Deposit>>& allDeposits,
        const std::string& filename = "") override;
};
