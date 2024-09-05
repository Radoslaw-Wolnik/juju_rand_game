// File: common/strategies/adaptive_strategy.hpp

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
    AdaptiveStrategy(std::vector<std::unique_ptr<Strategy>> strats)
        : strategies(std::move(strats)), 
          strategy_weights(strategies.size(), 1.0 / strategies.size()),
          strategy_metrics(strategies.size(), {0.0, 0.0, 0}) {
        std::random_device rd;
        rng.seed(rd());
    }

    Move choose_move(const GameState& game_state) override {
        if (++games_since_adaptation >= ADAPTATION_INTERVAL) {
            adapt_weights();
            games_since_adaptation = 0;
        }

        std::discrete_distribution<> dist(strategy_weights.begin(), strategy_weights.end());
        int chosen_strategy = dist(rng);
        return strategies[chosen_strategy]->choose_move(game_state);
    }

    void update_metrics(int strategy_index, double score, bool win) {
        auto& metrics = strategy_metrics[strategy_index];
        metrics.avg_score = (metrics.avg_score * metrics.games_played + score) / (metrics.games_played + 1);
        metrics.win_rate = (metrics.win_rate * metrics.games_played + (win ? 1 : 0)) / (metrics.games_played + 1);
        metrics.games_played++;
    }

private:
    void adapt_weights() {
        double total_performance = 0.0;
        for (const auto& metrics : strategy_metrics) {
            double performance = metrics.win_rate * 0.7 + (1.0 / metrics.avg_score) * 0.3;
            total_performance += performance;
        }

        for (size_t i = 0; i < strategies.size(); ++i) {
            double performance = strategy_metrics[i].win_rate * 0.7 + (1.0 / strategy_metrics[i].avg_score) * 0.3;
            strategy_weights[i] = performance / total_performance;
        }
    }

    std::string name() const override {
        return "Adaptive Strategy";
    }

    std::string description() const override {
        return "A strategy that adapts by choosing among multiple strategies based on their performance";
    }
};