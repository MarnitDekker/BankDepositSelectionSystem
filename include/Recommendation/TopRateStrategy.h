#pragma once
#include "IRecommendationStrategy.h"

class TopRateStrategy : public IRecommendationStrategy {
public:
    std::vector<std::shared_ptr<Deposit>> recommend(
        const std::vector<std::shared_ptr<Deposit>>& deposits,
        const Client& client) override;
};