#pragma once
#include "base_game.hpp"
#include <immintrin.h>  // For SIMD instructions

class UltraOptimizedBaseGame : public BaseGame {
private:
    alignas(64) std::array<uint16_t, 2> boards;
    alignas(64) std::array<uint32_t, 2> scores;
    alignas(64) std::array<uint16_t, 13> subsetCache;
    alignas(64) std::array<uint64_t, 4> xorshiftState;

    uint64_t xorshift128plus();
    uint8_t rollDice();
    uint16_t generateMove(uint16_t board, uint8_t roll);

public:
    UltraOptimizedBaseGame(StrategyPtr strategyA, StrategyPtr strategyB);
    void play() override;
    void playRound(int player) override;
};