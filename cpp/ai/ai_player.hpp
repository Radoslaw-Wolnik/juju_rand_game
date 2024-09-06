#pragma once
#include "../cpp/include/strategies.hpp"

class AIPlayer : public Strategy {
public:
    int chooseMove(unsigned short board, int roll) override;
    std::string getName() const override { return "AI Player"; }
};