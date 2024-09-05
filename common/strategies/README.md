# Adaptive Strategy

The Adaptive Strategy is a meta-strategy that dynamically selects from a pool of other strategies based on their performance.

## Features

- Maintains a collection of multiple strategies
- Dynamically adjusts selection probabilities based on performance
- Considers both win rate and average score in performance evaluation
- Adapts strategy selection every 100 games

## Usage

To use the Adaptive Strategy:

1. Implement various strategies that conform to the `Strategy` interface.
2. Create an instance of `AdaptiveStrategy` with these strategies:

```cpp
std::vector<std::unique_ptr<Strategy>> strategies;
strategies.push_back(std::make_unique<RandomStrategy>());
strategies.push_back(std::make_unique<GreedyStrategy>());
// Add more strategies...

AdaptiveStrategy adaptive_strategy(std::move(strategies));
```

3. Use the `adaptive_strategy` in your game simulations.

## Customization

You can customize the adaptive strategy by modifying:

- `ADAPTATION_INTERVAL`: Change how frequently the strategy adapts.
- Performance metric calculation: Modify the weights for win rate and average score in the `adapt_weights` method.
- Add new performance metrics: Extend the `PerformanceMetrics` struct and update the `update_metrics` and `adapt_weights` methods.

## Future Improvements

- Implement different adaptation algorithms (e.g., epsilon-greedy, UCB1).
- Add state-dependent strategy selection.
- Implement online learning algorithms to continuously update strategy parameters.