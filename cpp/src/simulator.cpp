#include "simulator.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

Simulator::Simulator(const SimulationConfig& config, int* argc, char*** argv)
    : config(config), rng(config.seed) {
    MPI_Init(argc, argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    for (const auto& strategy_name : config.strategies) {
        strategy_instances.push_back(createStrategy(strategy_name));
    }
}

Simulator::~Simulator() {
    MPI_Finalize();
}

std::unique_ptr<Game> Simulator::createGame(std::shared_ptr<Strategy> strategy_a, std::shared_ptr<Strategy> strategy_b) {
    if (config.optimization_level == OptimizationLevel::UltraOptimized) {
        return std::make_unique<UltraOptimizedGame>(strategy_a, strategy_b, config.game_mode);
    } else {
        return std::make_unique<Game>(strategy_a, strategy_b, config.game_mode, config.optimization_level);
    }
}

GameResult Simulator::runSingleGame(std::shared_ptr<Strategy> strategy_a, std::shared_ptr<Strategy> strategy_b) {
    auto game = createGame(strategy_a, strategy_b);
    game->play();
    GameResult result;
    result.scores[0] = game->getPlayerState(0).score;
    result.scores[1] = game->getPlayerState(1).score;
    result.winner = (result.scores[0] < result.scores[1]) ? 0 : 1;
    result.num_rounds = game->getCurrentRound();
    return result;
}

void Simulator::runSimulationsThread(int games_per_thread, std::vector<GameResult>& results) {
    for (int i = 0; i < games_per_thread; ++i) {
        results.push_back(runSingleGame(strategy_instances[0], strategy_instances[1]));
        games_completed++;
    }
}

nlohmann::json Simulator::runSimulation() {
    int games_per_process = config.num_games / world_size;
    int num_threads = std::thread::hardware_concurrency();
    int games_per_thread = games_per_process / num_threads;

    std::vector<std::vector<GameResult>> thread_results(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(&Simulator::runSimulationsThread, this, games_per_thread, std::ref(thread_results[i]));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::vector<GameResult> local_results;
    for (const auto& thread_result : thread_results) {
        local_results.insert(local_results.end(), thread_result.begin(), thread_result.end());
    }

    std::vector<GameResult> global_results;
    if (world_rank == 0) {
        global_results.resize(config.num_games);
    }

    MPI_Gather(local_results.data(), games_per_process * sizeof(GameResult), MPI_BYTE,
               global_results.data(), games_per_process * sizeof(GameResult), MPI_BYTE,
               0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        nlohmann::json json_results;
        json_results["config"] = {
            {"num_games", config.num_games},
            {"game_mode", config.game_mode == GameMode::Base ? "base" : "extended"},
            {"optimization_level", static_cast<int>(config.optimization_level)},
            {"strategies", config.strategies},
            {"seed", config.seed}
        };

        for (const auto& result : global_results) {
            json_results["games"].push_back({
                {"winner", result.winner},
                {"scores", result.scores},
                {"num_rounds", result.num_rounds}
            });
        }

        return json_results;
    }

    return nlohmann::json();
}