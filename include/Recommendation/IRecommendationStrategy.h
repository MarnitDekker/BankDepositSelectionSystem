#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Client.h"
#include "Deposit.h"

class IRecommendationStrategy {
public:
    virtual ~IRecommendationStrategy() = default;
    virtual std::vector<std::shared_ptr<Deposit>> recommend(
        const std::vector<std::shared_ptr<Deposit>>& deposits,
        const Client& client) = 0;
};