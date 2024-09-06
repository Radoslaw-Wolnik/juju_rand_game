#include "../include/ultra_optimized_base_game.hpp"
#include <algorithm>

UltraOptimizedBaseGame::UltraOptimizedBaseGame(StrategyPtr strategyA, StrategyPtr strategyB)
    : BaseGame(strategyA, strategyB) {
    std::fill(boards.begin(), boards.end(), 0xFFF);
    std::fill(scores.begin(), scores.end(), 0);
    for (int i = 2; i <= 12; ++i) {
        subsetCache[i] = (1 << i) - 1;
    }
    std::random_device rd;
    std::generate(xorshiftState.begin(), xorshiftState.end(), rd);
}

uint64_t UltraOptimizedBaseGame::xorshift128plus() {
    uint64_t x = xorshiftState[0];
    uint64_t const y = xorshiftState[1];
    xorshiftState[0] = y;
    x ^= x << 23;
    x ^= x >> 17;
    x ^= y ^ (y >> 26);
    xorshiftState[1] = x;
    return x + y;
}

uint8_t UltraOptimizedBaseGame::rollDice() {
    return ((xorshift128plus() & 0x7) % 6 + (xorshift128plus() & 0x38) % 6 + 2) as u8;
}

uint16_t UltraOptimizedBaseGame::generateMove(uint16_t board, uint8_t roll) {
    uint16_t move = board & (1 << (roll - 1));
    if (move) return move;
    uint16_t subset = subsetCache[roll] & board;
    return subset ? _blsi_u32(subset) : 0;  // Use BLSI instruction to get lowest set bit
}

void UltraOptimizedBaseGame::play() {
    for (int round = 0; round < 5; ++round) {
        for (int player = 0; player < 2; ++player) {
            playRound(player);
        }
    }
    // Print results...
}

void UltraOptimizedBaseGame::playRound(int player) {
    boards[player] = 0xFFF;
    while (boards[player]) {
        uint8_t roll = rollDice();
        uint16_t move = generateMove(boards[player], roll);
        if (!move) break;
        boards[player] &= ~move;
    }
    scores[player] += _mm_popcnt_u32(boards[player]);
}