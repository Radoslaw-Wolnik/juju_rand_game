#include "../include/ultra_fast_base_game.hpp"

UltraFastBaseGame::UltraFastBaseGame(StrategyPtr strategyA, StrategyPtr strategyB)
    : BaseGame(strategyA, strategyB), xorshift_state(std::random_device{}()) {}

int UltraFastBaseGame::rollDice() {
    xorshift_state ^= xorshift_state << 13;
    xorshift_state ^= xorshift_state >> 7;
    xorshift_state ^= xorshift_state << 17;
    return ((xorshift_state & 0xF) % 6 + 1) + ((xorshift_state >> 4 & 0xF) % 6 + 1);
}