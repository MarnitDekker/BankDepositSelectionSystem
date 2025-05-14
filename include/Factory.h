#pragma once
#include <memory>
#include "Recommendation/IRecommendationStrategy.h"
#include "Report/IReportGenerator.h"

class Factory {
public:
    enum class ReportType { HTML, TEXT };
    enum class StrategyType { TOP_RATE, FLEXIBLE_TERM };

    static std::unique_ptr<IRecommendationStrategy> createStrategy(StrategyType type);
    static std::unique_ptr<IReportGenerator> createReportGenerator(ReportType type);
};
