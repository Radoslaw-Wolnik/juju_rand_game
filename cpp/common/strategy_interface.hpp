
#pragma once
#include <vector>
#include <memory>
#include <string>

class Strategy {
public:
    virtual int chooseMove(unsigned short board, int roll) = 0;
    virtual std::string getName() const = 0;
    virtual ~Strategy() = default;
};

using StrategyPtr = std::shared_ptr<Strategy>;
