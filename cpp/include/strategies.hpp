#pragma once
#include <string>
#include <vector>
#include <memory>
#include <random>

class Strategy {
public:
    virtual uint16_t chooseMove(uint16_t board, uint8_t roll) = 0;
    virtual std::string getName() const = 0;
    virtual ~Strategy() = default;
};

class RandomStrategy : public Strategy {
public:
    uint16_t chooseMove(uint16_t board, uint8_t roll) override;
    std::string getName() const override { return "Random"; }
};

class HighestValueStrategy : public Strategy {
public:
    uint16_t chooseMove(uint16_t board, uint8_t roll) override;
    std::string getName() const override { return "HighestValue"; }
};

class HighestProbabilityStrategy : public Strategy {
public:
    uint16_t chooseMove(uint16_t board, uint8_t roll) override;
    std::string getName() const override { return "HighestProbability"; }
};

class BalancedStrategy : public Strategy {
public:
    uint16_t chooseMove(uint16_t board, uint8_t roll) override;
    std::string getName() const override { return "Balanced"; }
};

class AdaptiveStrategy : public Strategy {
private:
    std::vector<std::unique_ptr<Strategy>> strategies;
    std::vector<double> strategy_weights;
    std::mt19937 rng;

public:
    AdaptiveStrategy();
    uint16_t chooseMove(uint16_t board, uint8_t roll) override;
    std::string getName() const override { return "Adaptive"; }
};

std::shared_ptr<Strategy> createStrategy(const std::string& name);