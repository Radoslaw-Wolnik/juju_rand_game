#include "../include/distributed_simulator.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

DistributedSimulator::DistributedSimulator(int* argc, char*** argv, int total_games, 
                                           const std::string& game_type, const std::string& game_mode,
                                           StrategyPtr strategy_a, StrategyPtr strategy_b)
    : total_games(total_games), game_type(game_type), game_mode(game_mode),
      strategy_a(strategy_a), strategy_b(strategy_b) {
    MPI_Init(argc, argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
}

DistributedSimulator::~DistributedSimulator() {
    MPI_Finalize();
}

std::unique_ptr<BaseGame> DistributedSimulator::createGame() {
    if (game_type == "base") {
        if (game_mode == "ultra_optimized") {
            return std::make_unique<UltraOptimizedBaseGame>(strategy_a, strategy_b);
        } else {
            return std::make_unique<BaseGame>(strategy_a, strategy_b);
        }
    } else if (game_type == "extended") {
        if (game_mode == "ultra_optimized") {
            return std::make_unique<UltraOptimizedExtendedGame>(strategy_a, strategy_b);
        } else {
            return std::make_unique<ExtendedGame>(strategy_a, strategy_b);
        }
    }
    throw std::invalid_argument("Invalid game type or mode");
}

void DistributedSimulator::runSimulations(int num_games, std::vector<GameResult>& results) {
    for (int i = 0; i < num_games; ++i) {
        auto game = createGame();
        game->play();
        GameResult result;
        result.score_a = game->getScore(0);
        result.score_b = game->getScore(1);
        result.winner = (result.score_a < result.score_b) ? 0 : 1;
        result.num_rounds = game->getCurrentRound();
        results.push_back(result);
    }
}

void DistributedSimulator::run() {
    int games_per_process = total_games / world_size;
    std::vector<GameResult> local_results;
    runSimulations(games_per_process, local_results);

    std::vector<GameResult> global_results;
    if (world_rank == 0) {
        global_results.resize(total_games);
    }

    MPI_Gather(local_results.data(), games_per_process * sizeof(GameResult), MPI_BYTE,
               global_results.data(), games_per_process * sizeof(GameResult), MPI_BYTE,
               0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        nlohmann::json json_results;
        for (const auto& result : global_results) {
            json_results["games"].push_back({
                {"winner", result.winner},
                {"score_a", result.score_a},
                {"score_b", result.score_b},
                {"num_rounds", result.num_rounds}
            });
        }

        std::ofstream output_file("simulation_results.json");
        output_file << json_results.dump(4);
        std::cout << "Simulation results saved to simulation_results.json" << std::endl;
    }
}