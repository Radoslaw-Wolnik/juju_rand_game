#pragma once
#include "strategy_interface.hpp"
#include <vector>
#include <memory>
#include <random>

class AdaptiveStrategy : public Strategy {
private:
    std::vector<std::unique_ptr<Strategy>> strategies;
    std::vector<double> strategy_weights;
    std::mt19937 rng;

    struct PerformanceMetrics {
        double avg_score;
        double win_rate;
        int games_played;
    };
    std::vector<PerformanceMetrics> strategy_metrics;

    const int ADAPTATION_INTERVAL = 100; // Adapt every 100 games
    int games_since_adaptation = 0;

public:
    AdaptiveStrategy(std::vector<std::unique_ptr<Strategy>> strats);
    Move choose_move(const GameState& game_state) override;
    void update_metrics(int strategy_index, double score, bool win);
    std::string name() const override;
    std::string description() const override;

private:
    void adapt_weights();
};