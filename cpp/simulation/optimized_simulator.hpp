// File: cpp/include/optimized_simulator.hpp
#pragma once
#include "ultra_optimized_base_game.hpp"
#include "ultra_optimized_extended_game.hpp"
#include "strategies.hpp"
#include <vector>
#include <random>

class OptimizedSimulator {
private:
    std::mt19937 rng;
    std::vector<StrategyPtr> strategies;

    // Use the ultra-optimized game implementations
    std::unique_ptr<UltraOptimizedBaseGame> baseGame;
    std::unique_ptr<UltraOptimizedExtendedGame> extendedGame;

    struct SimulationResult {
        int winnerIndex;
        std::array<int, 2> scores;
        int numRounds;
    };

public:
    OptimizedSimulator(uint64_t seed);
    void addStrategy(StrategyPtr strategy);
    SimulationResult runBaseGameSimulation(int strategyIndexA, int strategyIndexB);
    SimulationResult runExtendedGameSimulation(int strategyIndexA, int strategyIndexB);
};