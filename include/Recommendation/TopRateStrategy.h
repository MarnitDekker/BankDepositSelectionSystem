#pragma once
#include "IRecommendationStrategy.h"
#include <iterator>

class TopRateStrategy : public IRecommendationStrategy {
public:
    ~TopRateStrategy() override = default;

    std::vector<std::shared_ptr<Deposit>> recommend(
        const std::vector<std::shared_ptr<Deposit>>& deposits,
        const Client& client) override;
};