#include "game.hpp"
#include <algorithm>
#include <random>

Game::Game(std::shared_ptr<Strategy> strategy_a, std::shared_ptr<Strategy> strategy_b,
           GameMode mode, OptimizationLevel optimization)
    : strategies{strategy_a, strategy_b}, mode(mode), optimization(optimization),
      player_states{GameState{0xFFF, 0, 0}, GameState{0xFFF, 0, 0}},
      rng(std::random_device{}()) {}

uint8_t Game::rollDice() {
    return std::uniform_int_distribution<>(1, 6)(rng) + std::uniform_int_distribution<>(1, 6)(rng);
}

uint16_t Game::generateMove(uint16_t board, uint8_t roll) {
    uint16_t move = board & (1 << (roll - 1));
    if (move) return move;
    uint16_t subset = ((1 << roll) - 1) & board;
    return subset ? subset & (-subset) : 0;  // BLSI instruction equivalent
}

void Game::play() {
    if (mode == GameMode::Base) {
        for (int round = 0; round < 5; ++round) {
            for (int player = 0; player < 2; ++player) {
                playRound(player);
            }
        }
    } else {  // Extended mode
        int current_player = 0;
        while (true) {
            if (!makeMove(current_player, rollDice())) {
                current_player = 1 - current_player;
                continue;
            }
            if (player_states[current_player].board == 0 || player_states[current_player].board == 0xFFF) {
                break;
            }
            current_player = 1 - current_player;
        }
    }
}

void Game::playRound(int player) {
    player_states[player].board = 0xFFF;
    while (makeMove(player, rollDice())) {}
    player_states[player].score += __builtin_popcount(player_states[player].board);
    player_states[player].round++;
}

bool Game::makeMove(int player, uint8_t roll) {
    uint16_t move = strategies[player]->chooseMove(player_states[player].board, roll);
    if (move == 0) return false;
    if (mode == GameMode::Base || player == 0) {
        player_states[player].board &= ~move;
    } else {
        player_states[player].board |= move;
    }
    return true;
}

// UltraOptimizedGame implementation
UltraOptimizedGame::UltraOptimizedGame(std::shared_ptr<Strategy> strategy_a, std::shared_ptr<Strategy> strategy_b, GameMode mode)
    : Game(strategy_a, strategy_b, mode, OptimizationLevel::UltraOptimized) {
    std::random_device rd;
    std::generate(xorshift_state.begin(), xorshift_state.end(), rd);
    for (int i = 2; i <= 12; ++i) {
        subset_cache[i] = (1 << i) - 1;
    }
}

uint64_t UltraOptimizedGame::xorshift128plus() {
    uint64_t x = xorshift_state[0];
    uint64_t const y = xorshift_state[1];
    xorshift_state[0] = y;
    x ^= x << 23;
    x ^= x >> 17;
    x ^= y ^ (y >> 26);
    xorshift_state[1] = x;
    return x + y;
}

uint8_t UltraOptimizedGame::rollDice() {
    return ((xorshift128plus() & 0x7) % 6 + (xorshift128plus() & 0x38) % 6 + 2);
}

uint16_t UltraOptimizedGame::generateMove(uint16_t board, uint8_t roll) {
    uint16_t move = board & (1 << (roll - 1));
    if (move) return move;
    uint16_t subset = subset_cache[roll] & board;
    return subset ? __builtin_ctz(subset) : 0;  // Use TZCNT instruction for efficiency
}

void UltraOptimizedGame::play() {
    if (mode == GameMode::Base) {
        for (int round = 0; round < 5; ++round) {
            for (int player = 0; player < 2; ++player) {
                playRound(player);
            }
        }
    } else {
        Game::play();  // Use the base class implementation for extended mode
    }
}

void UltraOptimizedGame::playRound(int player) {
    player_states[player].board = 0xFFF;
    while (player_states[player].board) {
        uint8_t roll = rollDice();
        uint16_t move = generateMove(player_states[player].board, roll);
        if (!move) break;
        player_states[player].board &= ~move;
    }
    player_states[player].score += __builtin_popcount(player_states[player].board);
    player_states[player].round++;
}