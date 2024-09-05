#pragma once
#include "base_game.hpp"

class UltraFastBaseGame : public BaseGame {
private:
    unsigned long long xorshift_state;

public:
    UltraFastBaseGame(StrategyPtr strategyA, StrategyPtr strategyB);
    int rollDice() override;
};