#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include "../../cpp/include/base_game.hpp"

class MultiThreadedSimulator {
private:
    std::vector<std::thread> threads;
    std::atomic<int> gamesCompleted{0};
    int totalGames;
    int numThreads;

    void runSimulations(int gamesPerThread, StrategyPtr strategyA, StrategyPtr strategyB);

public:
    MultiThreadedSimulator(int totalGames, int numThreads);
    void run(StrategyPtr strategyA, StrategyPtr strategyB);
    int getGamesCompleted() const;
};