#pragma once
#include <array>
#include <memory>
#include <random>
#include "strategies.hpp"

enum class GameMode { Base, Extended };
enum class OptimizationLevel { Standard, Fast, UltraOptimized };

struct GameState {
    uint16_t board;
    uint32_t score;
    uint8_t round;
};

class Game {
protected:
    std::array<std::shared_ptr<Strategy>, 2> strategies;
    GameMode mode;
    OptimizationLevel optimization;
    std::array<GameState, 2> player_states;
    std::mt19937 rng;

    virtual uint8_t rollDice();
    virtual uint16_t generateMove(uint16_t board, uint8_t roll);

public:
    Game(std::shared_ptr<Strategy> strategy_a, std::shared_ptr<Strategy> strategy_b,
         GameMode mode, OptimizationLevel optimization);

    virtual void play();
    virtual void playRound(int player);
    virtual bool makeMove(int player, uint8_t roll);

    GameState getPlayerState(int player) const { return player_states[player]; }
    int getCurrentRound() const { return player_states[0].round; }
};

class UltraOptimizedGame : public Game {
private:
    std::array<uint64_t, 2> xorshift_state;
    std::array<uint16_t, 13> subset_cache;

    uint64_t xorshift128plus();
    uint8_t rollDice() override;
    uint16_t generateMove(uint16_t board, uint8_t roll) override;

public:
    UltraOptimizedGame(std::shared_ptr<Strategy> strategy_a, std::shared_ptr<Strategy> strategy_b, GameMode mode);
    void play() override;
    void playRound(int player) override;
};