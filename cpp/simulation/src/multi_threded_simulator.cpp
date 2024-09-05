#include "../include/multi_threaded_simulator.hpp"

MultiThreadedSimulator::MultiThreadedSimulator(int totalGames, int numThreads)
    : totalGames(totalGames), numThreads(numThreads) {}

void MultiThreadedSimulator::runSimulations(int gamesPerThread, StrategyPtr strategyA, StrategyPtr strategyB) {
    for (int i = 0; i < gamesPerThread; ++i) {
        BaseGame game(strategyA, strategyB);
        game.play();
        gamesCompleted++;
    }
}

void MultiThreadedSimulator::run(StrategyPtr strategyA, StrategyPtr strategyB) {
    int gamesPerThread = totalGames / numThreads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(&MultiThreadedSimulator::runSimulations, this, gamesPerThread, strategyA, strategyB);
    }
    for (auto& thread : threads) {
        thread.join();
    }
}

int MultiThreadedSimulator::getGamesCompleted() const {
    return gamesCompleted;
}