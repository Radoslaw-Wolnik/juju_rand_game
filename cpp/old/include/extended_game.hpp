#pragma once
#include "base_game.hpp"

class ExtendedGame : public BaseGame {
public:
    ExtendedGame(StrategyPtr strategyA, StrategyPtr strategyB);
    void play() override;
    bool makeMove(int player, int roll) override;
};