// File: simulation/parallel/distributed_simulator.cpp
//#include "distributed_simulator.hpp"
#include "optimized_simulator.hpp"
#include <mpi.h>

void DistributedSimulator::runSimulations(int num_games, std::vector<GameResult>& results) {
    OptimizedSimulator simulator(world_rank); // Use rank as seed for reproducibility
    
    for (const auto& strategy : strategies) {
        simulator.addStrategy(strategy);
    }

    for (int i = 0; i < num_games; ++i) {
        auto result = simulator.runBaseGameSimulation(0, 1); // Assuming we're comparing first two strategies
        
        GameResult gameResult;
        gameResult.winner = result.winnerIndex;
        gameResult.score_a = result.scores[0];
        gameResult.score_b = result.scores[1];
        gameResult.num_rounds = result.numRounds;
        
        results.push_back(gameResult);
    }
}