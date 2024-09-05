#include "../include/interactive_games.hpp"
#include <iostream>
#include <string>
#include <limits>

int main() {
    std::string gameType, gameMode, aiStrategy;

    std::cout << "Select game type (base/extended): ";
    std::cin >> gameType;

    std::cout << "Select game mode (base/ultra_fast/balanced_probability): ";
    std::cin >> gameMode;

    std::cout << "Select AI strategy (Random/HighestValue/HighestProbability/Balanced/Composite/Adaptive): ";
    std::cin >> aiStrategy;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

    try {
        if (gameType == "base") {
            InteractiveBaseGame game(gameMode, aiStrategy);
            game.play();
        } else if (gameType == "extended") {
            InteractiveExtendedGame game(gameMode, aiStrategy);
            game.play();
        } else {
            std::cout << "Invalid game type selected." << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}