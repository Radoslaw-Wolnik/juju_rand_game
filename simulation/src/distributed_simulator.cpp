// File: cpp/src/comprehensive_simulator.cpp
#include "optimized_simulator.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

void runComprehensiveSimulations(int numGames, const std::vector<StrategyPtr>& strategies) {
    OptimizedSimulator simulator(std::random_device{}());
    
    for (const auto& strategy : strategies) {
        simulator.addStrategy(strategy);
    }

    nlohmann::json results;

    for (size_t i = 0; i < strategies.size(); ++i) {
        for (size_t j = i + 1; j < strategies.size(); ++j) {
            std::cout << "Simulating " << strategies[i]->getName() << " vs " << strategies[j]->getName() << std::endl;

            int winsA = 0, winsB = 0;
            int64_t totalScoreA = 0, totalScoreB = 0;
            int64_t totalRounds = 0;

            for (int game = 0; game < numGames; ++game) {
                auto baseResult = simulator.runBaseGameSimulation(i, j);
                winsA += (baseResult.winnerIndex == 0);
                winsB += (baseResult.winnerIndex == 1);
                totalScoreA += baseResult.scores[0];
                totalScoreB += baseResult.scores[1];
                totalRounds += baseResult.numRounds;

                auto extendedResult = simulator.runExtendedGameSimulation(i, j);
                winsA += (extendedResult.winnerIndex == 0);
                winsB += (extendedResult.winnerIndex == 1);
                totalRounds += extendedResult.numRounds;
            }

            results[strategies[i]->getName() + "_vs_" + strategies[j]->getName()] = {
                {"base_game", {
                    {"player_a_wins", winsA},
                    {"player_b_wins", winsB},
                    {"player_a_avg_score", totalScoreA / (2.0 * numGames)},
                    {"player_b_avg_score", totalScoreB / (2.0 * numGames)}
                }},
                {"extended_game", {
                    {"player_a_wins", winsA},
                    {"player_b_wins", winsB}
                }},
                {"avg_rounds", totalRounds / (2.0 * numGames)}
            };
        }
    }

    std::ofstream output("simulation_results.json");
    output << results.dump(4);
    std::cout << "Results saved to simulation_results.json" << std::endl;
}

int main() {
    std::vector<StrategyPtr> strategies = {
        std::make_shared<RandomStrategy>(),
        std::make_shared<HighestValueStrategy>(),
        std::make_shared<HighestProbabilityStrategy>(),
        std::make_shared<BalancedStrategy>(),
        std::make_shared<AdaptiveStrategy>()
    };

    runComprehensiveSimulations(10000, strategies);
    return 0;
}