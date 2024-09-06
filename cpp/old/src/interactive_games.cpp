#include "../include/interactive_games.hpp"
#include <limits>

InteractiveBaseGame::InteractiveBaseGame(const std::string& gameMode, const std::string& aiStrategyName) {
    aiStrategy = createStrategy(aiStrategyName);
    StrategyPtr humanStrategy = std::make_shared<Strategy>(); // Dummy strategy for human player

    if (gameMode == "base") {
        game = std::make_unique<BaseGame>(humanStrategy, aiStrategy);
    } else if (gameMode == "ultra_fast") {
        game = std::make_unique<UltraFastGame>(humanStrategy, aiStrategy);
    } else if (gameMode == "balanced_probability") {
        game = std::make_unique<BalancedProbabilityGame>(humanStrategy, aiStrategy);
    } else {
        throw std::invalid_argument("Invalid game mode");
    }
}

void InteractiveBaseGame::play() {
    for (int round = 0; round < 5; ++round) {
        std::cout << "Round " << round + 1 << " begins:" << std::endl;
        
        // Human player's turn
        game->board = 0xFFF;
        while (true) {
            int roll = game->rollDice();
            std::cout << "You rolled: " << roll << std::endl;
            displayBoard();
            if (!makePlayerMove(roll)) break;
        }
        int humanScore = __builtin_popcount(game->board);
        game->scores[0] += humanScore;
        std::cout << "Your score this round: " << humanScore << std::endl;

        // AI player's turn
        game->board = 0xFFF;
        game->playRound(1);
        std::cout << "AI score this round: " << game->scores[1] - game->scores[0] << std::endl;

        std::cout << "Current total: You = " << game->scores[0] << ", AI = " << game->scores[1] << std::endl << std::endl;
    }

    std::cout << "Game over. Final scores: You = " << game->scores[0] 
              << ", AI = " << game->scores[1] << std::endl;
    std::cout << "Winner: " << (game->scores[0] < game->scores[1] ? "You" : "AI") << std::endl;
}

bool InteractiveBaseGame::makePlayerMove(int roll) {
    std::cout << "Enter the number(s) you want to flip (comma-separated, or 0 to end turn): ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "0") return false;

    std::istringstream iss(input);
    std::string token;
    int sum = 0;
    uint16_t move = 0;

    while (std::getline(iss, token, ',')) {
        int num = std::stoi(token);
        if (num < 1 || num > 12 || !(game->board & (1 << (num - 1)))) {
            std::cout << "Invalid move. Try again." << std::endl;
            return true;
        }
        sum += num;
        move |= 1 << (num - 1);
    }

    if (sum != roll) {
        std::cout << "Sum of flipped numbers must equal the roll. Try again." << std::endl;
        return true;
    }

    game->board &= ~move;
    return true;
}

void InteractiveBaseGame::displayBoard() const {
    std::cout << "Current board: ";
    for (int i = 11; i >= 0; --i) {
        std::cout << ((game->board & (1 << i)) ? '1' : '0');
    }
    std::cout << std::endl;
}

InteractiveExtendedGame::InteractiveExtendedGame(const std::string& gameMode, const std::string& aiStrategyName)
    : InteractiveBaseGame(gameMode, aiStrategyName) {
    // Replace the base game with an extended game
    StrategyPtr humanStrategy = std::make_shared<Strategy>(); // Dummy strategy for human player
    if (gameMode == "base") {
        game = std::make_unique<ExtendedGame>(humanStrategy, aiStrategy);
    } else if (gameMode == "ultra_fast") {
        game = std::make_unique<UltraFastExtendedGame>(humanStrategy, aiStrategy);
    } else if (gameMode == "balanced_probability") {
        game = std::make_unique<BalancedProbabilityExtendedGame>(humanStrategy, aiStrategy);
    } else {
        throw std::invalid_argument("Invalid game mode");
    }
}

void InteractiveExtendedGame::play() {
    bool isPlayerTurn = true;
    while (true) {
        int roll = game->rollDice();
        std::cout << (isPlayerTurn ? "Your" : "AI's") << " turn. Roll: " << roll << std::endl;
        displayBoard();

        bool moveMade;
        if (isPlayerTurn) {
            moveMade = makePlayerMove(roll);
        } else {
            moveMade = game->makeMove(1, roll);
            std::cout << "AI made a move." << std::endl;
        }

        if (!moveMade) {
            isPlayerTurn = !isPlayerTurn;
            continue;
        }

        if (game->board == 0) {
            std::cout << "You win!" << std::endl;
            break;
        } else if (game->board == 0xFFF) {
            std::cout << "AI wins!" << std::endl;
            break;
        }

        isPlayerTurn = !isPlayerTurn;
    }
}

bool InteractiveExtendedGame::makePlayerMove(int roll) {
    std::cout << "Enter the number(s) you want to flip off (comma-separated, or 0 to skip turn): ";
    std::string input;
    std::getline(std::cin, input);

    if (input == "0") return false;

    std::istringstream iss(input);
    std::string token;
    int sum = 0;
    uint16_t move = 0;

    while (std::getline(iss, token, ',')) {
        int num = std::stoi(token);
        if (num < 1 || num > 12 || !(game->board & (1 << (num - 1)))) {
            std::cout << "Invalid move. Try again." << std::endl;
            return true;
        }
        sum += num;
        move |= 1 << (num - 1);
    }

    if (sum != roll) {
        std::cout << "Sum of flipped numbers must equal the roll. Try again." << std::endl;
        return true;
    }

    game->board &= ~move;
    return true;
}