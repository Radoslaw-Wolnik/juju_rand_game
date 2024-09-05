#include "../include/base_game.hpp"
#include "../include/extended_game.hpp"
#include "../include/ultra_fast_base_game.hpp"
#include "../include/ultra_fast_extended_game.hpp"
#include "../include/balanced_probability_base_game.hpp"
#include "../include/balanced_probability_extended_game.hpp"
#include "../include/strategies.hpp"
#include "../include/distributed_simulator.hpp"
#include <iostream>
#include <string>
#include <memory>

int main(int argc, char** argv) {
    std::string gameType, gameMode, strategyA, strategyB;
    int totalGames;

    std::cout << "Select game type (base/extended): ";
    std::cin >> gameType;

    std::cout << "Select game mode (base/ultra_fast/balanced_probability): ";
    std::cin >> gameMode;

    std::cout << "Select strategy for Player A: ";
    std::cin >> strategyA;

    std::cout << "Select strategy for Player B: ";
    std::cin >> strategyB;

    std::cout << "Enter total number of games to simulate: ";
    std::cin >> totalGames;

    StrategyPtr playerAStrategy = createStrategy(strategyA);
    StrategyPtr playerBStrategy = createStrategy(strategyB);

    try {
        DistributedSimulator simulator(&argc, &argv, totalGames, gameType, gameMode, playerAStrategy, playerBStrategy);
        simulator.run();
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}