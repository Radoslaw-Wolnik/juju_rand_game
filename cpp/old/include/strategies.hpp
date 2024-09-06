#pragma once
#include "../../common/strategies/strategy_interface.hpp"
#include <vector>
#include <memory>
#include <random>
#include "adaptive_strategy.hpp"

class RandomStrategy : public Strategy {
public:
    int chooseMove(unsigned short board, int roll) override;
    std::string getName() const override { return "Random"; }
};

class HighestValueStrategy : public Strategy {
public:
    int chooseMove(unsigned short board, int roll) override;
    std::string getName() const override { return "HighestValue"; }
};

class HighestProbabilityStrategy : public Strategy {
public:
    int chooseMove(unsigned short board, int roll) override;
    std::string getName() const override { return "HighestProbability"; }
};

class BalancedStrategy : public Strategy {
public:
    int chooseMove(unsigned short board, int roll) override;
    std::string getName() const override { return "Balanced"; }
};

class CompositeStrategy : public Strategy {
private:
    std::vector<StrategyPtr> strategies;
    std::mt19937 rng;

public:
    CompositeStrategy(const std::vector<StrategyPtr>& strats);
    int chooseMove(unsigned short board, int roll) override;
    std::string getName() const override { return "Composite"; }
};

StrategyPtr createStrategy(const std::string& name);

