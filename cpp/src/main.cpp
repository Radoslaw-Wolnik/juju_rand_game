#include "simulator.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    SimulationConfig config{
        .num_games = 10000,
        .game_mode = GameMode::Base,
        .optimization_level = OptimizationLevel::UltraOptimized,
        .strategies = {"Random", "HighestValue"},
        .seed = 12345
    };

    Simulator simulator(config, &argc, &argv);
    nlohmann::json results = simulator.runSimulation();

    if (results.is_null()) {
        // Non-root processes exit here
        return 0;
    }

    // Root process saves the results
    std::ofstream output_file("simulation_results.json");
    output_file << results.dump(4);
    std::cout << "Simulation results saved to simulation_results.json" << std::endl;

    return 0;
}