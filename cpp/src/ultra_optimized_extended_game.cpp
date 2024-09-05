#include "../include/ultra_optimized_extended_game.hpp"
#include <iostream>

UltraOptimizedExtendedGame::UltraOptimizedExtendedGame(StrategyPtr strategyA, StrategyPtr strategyB)
    : UltraOptimizedBaseGame(strategyA, strategyB) {}

void UltraOptimizedExtendedGame::play() {
    int currentPlayer = 0;
    while (true) {
        uint8_t roll = rollDice();
        if (!makeMove(currentPlayer, roll)) {
            currentPlayer = 1 - currentPlayer;
            continue;
        }
        if (boards[currentPlayer] == 0 || boards[currentPlayer] == 0xFFF) {
            break;
        }
        currentPlayer = 1 - currentPlayer;
    }
    std::cout << "Game over. Winner: Player " << (boards[0] == 0 ? "A" : "B") << std::endl;
}

bool UltraOptimizedExtendedGame::makeMove(int player, uint8_t roll) {
    uint16_t move = strategies[player]->chooseMove(boards[player], roll);
    if (move == 0) return false;
    boards[player] = (player == 0) ? (boards[player] & ~move) : (boards[player] | move);
    return true;
}