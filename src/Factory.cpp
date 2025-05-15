#include "Factory.h"
#include "Recommendation/TopRateStrategy.h"
#include "Recommendation/FlexibleTermStrategy.h"
#include "Report/HTMLReportGenerator.h"
#include "Report/TextReportGenerator.h"

std::unique_ptr<IRecommendationStrategy> Factory::createStrategy(StrategyType type) {
    switch (type) {
    //case StrategyType::TOP_RATE:
    //    return std::make_unique<TopRateStrategy>();
    case StrategyType::FLEXIBLE_TERM:
        return std::make_unique<FlexibleTermStrategy>();
    default:
        return nullptr;
    }
}

std::unique_ptr<IReportGenerator> Factory::createReportGenerator(ReportType type) {
    switch (type) {
    case ReportType::HTML:
        return std::make_unique<HTMLReportGenerator>();
    case ReportType::TEXT:
        return std::make_unique<TextReportGenerator>();
    default:
        return nullptr;
    }
}