#include "../include/base_game.hpp"
#include <iostream>

BaseGame::BaseGame(StrategyPtr strategyA, StrategyPtr strategyB)
    : board(0xFFF), scores{0, 0}, currentRound(0),
      rng(std::random_device{}()), dice_dist(1, 6) {
    strategies[0] = strategyA;
    strategies[1] = strategyB;
}

void BaseGame::play() {
    for (currentRound = 0; currentRound < 5; ++currentRound) {
        for (int player = 0; player < 2; ++player) {
            playRound(player);
        }
    }
    std::cout << "Final scores: Player A = " << scores[0] << ", Player B = " << scores[1] << std::endl;
    std::cout << "Winner: Player " << (scores[0] < scores[1] ? "A" : "B") << std::endl;
}

void BaseGame::playRound(int player) {
    board = 0xFFF;
    while (makeMove(player, rollDice())) {}
    scores[player] += __builtin_popcount(board);
}

bool BaseGame::makeMove(int player, int roll) {
    int move = strategies[player]->chooseMove(board, roll);
    if (move == 0) return false;
    board &= ~move;
    return true;
}

int BaseGame::rollDice() {
    return dice_dist(rng) + dice_dist(rng);
}

int BaseGame::getScore(int player) const {
    return scores[player];
}