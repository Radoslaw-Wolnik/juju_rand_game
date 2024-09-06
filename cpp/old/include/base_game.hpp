#pragma once
#include "strategies.hpp"
#include <random>

class BaseGame {
protected:
    unsigned short board;
    int scores[2];
    int currentRound;
    StrategyPtr strategies[2];
    std::mt19937 rng;
    std::uniform_int_distribution<> dice_dist;

public:
    BaseGame(StrategyPtr strategyA, StrategyPtr strategyB);
    virtual void play();
    virtual void playRound(int player);
    virtual bool makeMove(int player, int roll);
    virtual int rollDice();
    int getScore(int player) const;
};