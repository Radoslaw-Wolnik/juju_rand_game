#include "strategies.hpp"
#include <algorithm>
#include <cmath>

uint16_t RandomStrategy::chooseMove(uint16_t board, uint8_t roll) {
    std::vector<uint16_t> valid_moves;
    for (int i = 1; i <= roll; ++i) {
        if (board & (1 << (i - 1))) {
            valid_moves.push_back(1 << (i - 1));
        }
    }
    if (valid_moves.empty()) return 0;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, valid_moves.size() - 1);
    return valid_moves[dis(gen)];
}

uint16_t HighestValueStrategy::chooseMove(uint16_t board, uint8_t roll) {
    for (int i = roll; i >= 1; --i) {
        if (board & (1 << (i - 1))) {
            return 1 << (i - 1);
        }
    }
    return 0;
}

uint16_t HighestProbabilityStrategy::chooseMove(uint16_t board, uint8_t roll) {
    static const std::vector<int> prob_order = {7, 6, 8, 5, 9, 4, 10, 3, 11, 2, 12};
    for (int i : prob_order) {
        if (i <= roll && (board & (1 << (i - 1)))) {
            return 1 << (i - 1);
        }
    }
    return 0;
}

uint16_t BalancedStrategy::chooseMove(uint16_t board, uint8_t roll) {
    std::vector<std::pair<uint16_t, double>> scores;
    for (int i = 1; i <= roll; ++i) {
        if (board & (1 << (i - 1))) {
            double prob_score = 7 - std::abs(7 - i);
            scores.emplace_back(1 << (i - 1), i * prob_score);
        }
    }
    if (scores.empty()) return 0;
    auto it = std::max_element(scores.begin(), scores.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    return it->first;
}

AdaptiveStrategy::AdaptiveStrategy() : rng(std::random_device{}()) {
    strategies.push_back(std::make_unique<RandomStrategy>());
    strategies.push_back(std::make_unique<HighestValueStrategy>());
    strategies.push_back(std::make_unique<HighestProbabilityStrategy>());
    strategies.push_back(std::make_unique<BalancedStrategy>());
    strategy_weights.resize(strategies.size(), 1.0 / strategies.size());
}

uint16_t AdaptiveStrategy::chooseMove(uint16_t board, uint8_t roll) {
    std::discrete_distribution<> dist(strategy_weights.begin(), strategy_weights.end());
    return strategies[dist(rng)]->chooseMove(board, roll);
}

std::shared_ptr<Strategy> createStrategy(const std::string& name) {
    if (name == "Random") return std::make_shared<RandomStrategy>();
    if (name == "HighestValue") return std::make_shared<HighestValueStrategy>();
    if (name == "HighestProbability") return std::make_shared<HighestProbabilityStrategy>();
    if (name == "Balanced") return std::make_shared<BalancedStrategy>();
    if (name == "Adaptive") return std::make_shared<AdaptiveStrategy>();
    throw std::invalid_argument("Unknown strategy name");
}