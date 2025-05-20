#include "FlexibleTermStrategy.h"
//#include <algorithm>
//#include <cmath>

std::vector<std::shared_ptr<Deposit>> FlexibleTermStrategy::recommend(
    const std::vector<std::shared_ptr<Deposit>>& deposits,
    const Client& client) {

    std::vector<std::shared_ptr<Deposit>> recommended;

    for (const auto& deposit : deposits) {
        if (deposit->getMinAmount() <= client.getAmount() &&
            abs(deposit->getTermMonths() - client.getTerm()) <= 3) {

            recommended.push_back(deposit);
        }
    }

    std::sort(recommended.begin(), recommended.end(),
        [&client](const auto& a, const auto& b) {
            return a->calculateIncome(client.getAmount()) >
                b->calculateIncome(client.getAmount());
        });

    if (recommended.size() > 5) {
        recommended.resize(5);
    }

    return recommended;
}