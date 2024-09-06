// File: cpp/src/optimized_simulator.cpp
#include "optimized_simulator.hpp"

OptimizedSimulator::OptimizedSimulator(uint64_t seed) : rng(seed) {
    baseGame = std::make_unique<UltraOptimizedBaseGame>(nullptr, nullptr);
    extendedGame = std::make_unique<UltraOptimizedExtendedGame>(nullptr, nullptr);
}

void OptimizedSimulator::addStrategy(StrategyPtr strategy) {
    strategies.push_back(strategy);
}

OptimizedSimulator::SimulationResult OptimizedSimulator::runBaseGameSimulation(int strategyIndexA, int strategyIndexB) {
    baseGame->setStrategies(strategies[strategyIndexA], strategies[strategyIndexB]);
    baseGame->play();
    return {
        baseGame->getWinner(),
        {baseGame->getScore(0), baseGame->getScore(1)},
        baseGame->getCurrentRound()
    };
}

OptimizedSimulator::SimulationResult OptimizedSimulator::runExtendedGameSimulation(int strategyIndexA, int strategyIndexB) {
    extendedGame->setStrategies(strategies[strategyIndexA], strategies[strategyIndexB]);
    extendedGame->play();
    return {
        extendedGame->getWinner(),
        {extendedGame->getScore(0), extendedGame->getScore(1)},
        extendedGame->getCurrentRound()
    };
}