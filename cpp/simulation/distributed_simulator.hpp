#pragma once
#include <mpi.h>
#include <vector>
#include <memory>
#include "../include/base_game.hpp"
#include "extended_game.hpp"
#include "ultra_optimized_base_game.hpp"
#include "ultra_optimized_extended_game.hpp"

class DistributedSimulator {
private:
    int world_size;
    int world_rank;
    int total_games;
    std::string game_type;
    std::string game_mode;
    StrategyPtr strategy_a;
    StrategyPtr strategy_b;

    std::unique_ptr<BaseGame> createGame();
    void runSimulations(int num_games, std::vector<GameResult>& results);

public:
    DistributedSimulator(int* argc, char*** argv, int total_games, 
                         const std::string& game_type, const std::string& game_mode,
                         StrategyPtr strategy_a, StrategyPtr strategy_b);
    ~DistributedSimulator();

    void run();
};

struct GameResult {
    int winner;
    int score_a;
    int score_b;
    int num_rounds;
};