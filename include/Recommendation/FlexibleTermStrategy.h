#pragma once
#include "IRecommendationStrategy.h"

class FlexibleTermStrategy : public IRecommendationStrategy {
public:
    ~FlexibleTermStrategy() override = default;

    std::vector<std::shared_ptr<Deposit>> recommend(
        const std::vector<std::shared_ptr<Deposit>>& deposits,
        const Client& client) override;
};