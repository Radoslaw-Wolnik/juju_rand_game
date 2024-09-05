#include "../include/extended_game.hpp"
#include <iostream>

ExtendedGame::ExtendedGame(StrategyPtr strategyA, StrategyPtr strategyB)
    : BaseGame(strategyA, strategyB) {}

void ExtendedGame::play() {
    int currentPlayer = 0;
    while (true) {
        if (!makeMove(currentPlayer, rollDice())) {
            currentPlayer = 1 - currentPlayer;
            continue;
        }
        if (board == 0 || board == 0xFFF) break;
        currentPlayer = 1 - currentPlayer;
    }
    std::cout << "Game over. Winner: Player " << (board == 0 ? "A" : "B") << std::endl;
}

bool ExtendedGame::makeMove(int player, int roll) {
    int move = strategies[player]->chooseMove(board, roll);
    if (move == 0) return false;
    board = (player == 0) ? (board & ~move) : (board | move);
    return true;
}