#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <nlohmann/json.hpp>
#include <fstream>

class MultithreadedSimulations {
private:
    std::vector<std::thread> threads;
    std::mutex resultMutex;
    std::atomic<int> gamesCompleted{0};
    nlohmann::json results;

    void runSimulation(int numGames, unsigned seed) {
        for (int i = 0; i < numGames; ++i) {
            FastBaseGame game(seed + i);
            auto gameResult = game.playAndGetResult();
            
            {
                std::lock_guard<std::mutex> lock(resultMutex);
                results["games"].push_back(gameResult);
            }
            
            gamesCompleted++;
        }
    }

public:
    void runSimulations(int totalGames, int numThreads) {
        int gamesPerThread = totalGames / numThreads;
        unsigned baseSeed = std::random_device{}();
        
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&MultithreadedSimulations::runSimulation, this, gamesPerThread, baseSeed + i * gamesPerThread);
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
        
        std::ofstream outputFile("simulation_results.json");
        outputFile << std::setw(4) << results << std::endl;
    }

    int getGamesCompleted() const {
        return gamesCompleted;
    }
};