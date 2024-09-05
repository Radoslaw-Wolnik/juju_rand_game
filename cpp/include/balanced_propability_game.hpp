#pragma once
#include "base_game.hpp"

class BalancedProbabilityBaseGame : public BaseGame {
private:
    std::discrete_distribution<> balanced_dist;

public:
    BalancedProbabilityBaseGame(StrategyPtr strategyA, StrategyPtr strategyB);
    int rollDice() override;
};