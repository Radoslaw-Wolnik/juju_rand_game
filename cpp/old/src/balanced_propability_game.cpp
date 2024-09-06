#include "../include/balanced_probability_base_game.hpp"

BalancedProbabilityBaseGame::BalancedProbabilityBaseGame(StrategyPtr strategyA, StrategyPtr strategyB)
    : BaseGame(strategyA, strategyB),
      balanced_dist({1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1}) {}

int BalancedProbabilityBaseGame::rollDice() {
    return balanced_dist(rng) + 2;
}