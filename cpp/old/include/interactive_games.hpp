#pragma once
#include "base_game.hpp"
#include "extended_game.hpp"
#include "ultra_fast_game.hpp"
#include "balanced_probability_game.hpp"
#include "strategies.hpp"
#include <memory>
#include <iostream>

class InteractiveBaseGame {
protected:
    std::unique_ptr<BaseGame> game;
    StrategyPtr aiStrategy;

public:
    InteractiveBaseGame(const std::string& gameMode, const std::string& aiStrategyName);
    void play();
    virtual bool makePlayerMove(int roll);
    void displayBoard() const;
};

class InteractiveExtendedGame : public InteractiveBaseGame {
public:
    InteractiveExtendedGame(const std::string& gameMode, const std::string& aiStrategyName);
    void play();
    bool makePlayerMove(int roll) override;
};