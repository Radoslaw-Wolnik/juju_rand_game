// File: common/strategies/strategy_interface.hpp

#pragma once

#include <string>
#include <cstdint>

struct GameState {
    uint16_t board;
    uint32_t current_round_score;
    uint32_t total_score;
    uint32_t opponent_score;
    size_t round;
};

struct Move {
    uint16_t bits_to_flip;
};

class Strategy {
public:
    virtual Move choose_move(const GameState& game_state) = 0;
    virtual std::string name() const = 0;
    virtual std::string description() const = 0;
    virtual ~Strategy() = default;
};