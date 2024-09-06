#pragma once
#include "game.hpp"
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <nlohmann/json.hpp>
#include <mpi.h>

struct SimulationConfig {
    int num_games;
    GameMode game_mode;
    OptimizationLevel optimization_level;
    std::vector<std::string> strategies;
    uint64_t seed;
};

struct GameResult {
    int winner;
    std::array<uint32_t, 2> scores;
    uint8_t num_rounds;
};

class Simulator {
private:
    SimulationConfig config;
    std::vector<std::shared_ptr<Strategy>> strategy_instances;
    std::mt19937 rng;
    int world_size;
    int world_rank;
    std::vector<std::thread> threads;
    std::atomic<int> games_completed{0};

    std::unique_ptr<Game> createGame(std::shared_ptr<Strategy> strategy_a, std::shared_ptr<Strategy> strategy_b);
    GameResult runSingleGame(std::shared_ptr<Strategy> strategy_a, std::shared_ptr<Strategy> strategy_b);
    void runSimulationsThread(int games_per_thread, std::vector<GameResult>& results);

public:
    Simulator(const SimulationConfig& config, int* argc, char*** argv);
    ~Simulator();
    nlohmann::json runSimulation();
};