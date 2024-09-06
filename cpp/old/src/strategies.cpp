#include "../include/strategies.hpp"
#include <algorithm>
#include <random>

int RandomStrategy::chooseMove(unsigned short board, int roll) {
    std::vector<int> validMoves;
    for (int i = 1; i <= roll; ++i) {
        if (board & (1 << (i - 1))) {
            validMoves.push_back(1 << (i - 1));
        }
    }
    if (validMoves.empty()) return 0;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validMoves.size() - 1);
    return validMoves[dis(gen)];
}

int HighestValueStrategy::chooseMove(unsigned short board, int roll) {
    for (int i = roll; i >= 1; --i) {
        if (board & (1 << (i - 1))) {
            return 1 << (i - 1);
        }
    }
    return 0;
}

int HighestProbabilityStrategy::chooseMove(unsigned short board, int roll) {
    static const std::vector<int> probOrder = {7, 6, 8, 5, 9, 4, 10, 3, 11, 2, 12};
    for (int i : probOrder) {
        if (i <= roll && (board & (1 << (i - 1)))) {
            return 1 << (i - 1);
        }
    }
    return 0;
}

int BalancedStrategy::chooseMove(unsigned short board, int roll) {
    std::vector<std::pair<int, double>> scores;
    for (int i = 1; i <= roll; ++i) {
        if (board & (1 << (i - 1))) {
            double probScore = 7 - std::abs(7 - i);
            scores.emplace_back(1 << (i - 1), i * probScore);
        }
    }
    if (scores.empty()) return 0;
    auto it = std::max_element(scores.begin(), scores.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });
    return it->first;
}

CompositeStrategy::CompositeStrategy(const std::vector<StrategyPtr>& strats)
    : strategies(strats), rng(std::random_device{}()) {}

int CompositeStrategy::chooseMove(unsigned short board, int roll) {
    std::uniform_int_distribution<> dis(0, strategies.size() - 1);
    return strategies[dis(rng)]->chooseMove(board, roll);
}

StrategyPtr createStrategy(const std::string& name) {
    if (name == "Random") return std::make_shared<RandomStrategy>();
    if (name == "HighestValue") return std::make_shared<HighestValueStrategy>();
    if (name == "HighestProbability") return std::make_shared<HighestProbabilityStrategy>();
    if (name == "Balanced") return std::make_shared<BalancedStrategy>();
    if (name == "Composite") {
        return std::make_shared<CompositeStrategy>(std::vector<StrategyPtr>{
            std::make_shared<RandomStrategy>(),
            std::make_shared<HighestValueStrategy>(),
            std::make_shared<HighestProbabilityStrategy>(),
            std::make_shared<BalancedStrategy>()
        });
    }
    if (name == "Adaptive") {
        std::vector<std::unique_ptr<Strategy>> subStrategies;
        subStrategies.push_back(std::make_unique<RandomStrategy>());
        subStrategies.push_back(std::make_unique<HighestValueStrategy>());
        subStrategies.push_back(std::make_unique<HighestProbabilityStrategy>());
        subStrategies.push_back(std::make_unique<BalancedStrategy>());
        return std::make_shared<AdaptiveStrategy>(std::move(subStrategies));
    }
    throw std::invalid_argument("Unknown strategy name");
}