#include "../Recommendation/TopRateStrategy.h"
#include <algorithm>

std::vector<std::shared_ptr<Deposit>> TopRateStrategy::recommend(
    const std::vector<std::shared_ptr<Deposit>>& deposits,
    const Client& client) {

    std::vector<std::shared_ptr<Deposit>> recommended(deposits);

    std::sort(recommended.begin(), recommended.end(),
        [](const auto& a, const auto& b) {
            return a->getInterestRate() > b->getInterestRate();
        });

    if (recommended.size() > 3) {
        recommended.resize(3);
    }

    return recommended;
}