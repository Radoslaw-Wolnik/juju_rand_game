#pragma once
#include "ultra_optimized_base_game.hpp"

class UltraOptimizedExtendedGame : public UltraOptimizedBaseGame {
public:
    UltraOptimizedExtendedGame(StrategyPtr strategyA, StrategyPtr strategyB);
    void play() override;
    bool makeMove(int player, uint8_t roll) override;
};