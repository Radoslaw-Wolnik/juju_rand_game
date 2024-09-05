# Distributed Simulator

This component provides a framework for running large-scale simulations of the dice game using distributed computing.

## Requirements

- MPI library (e.g., OpenMPI, MPICH)
- C++ compiler with C++17 support
- Rust compiler (for Rust version)

## Compilation

### C++ Version

```bash
mpic++ -std=c++17 -O3 distributed_simulator.cpp -o distributed_simulator
```

### Rust Version

Add the following to your `Cargo.toml`:

```toml
[dependencies]
mpi = "0.6"
```

Then build with:

```bash
cargo build --release
```

## Usage

Run the distributed simulator with:

```bash
mpirun -np <number_of_processes> ./distributed_simulator
```

Replace `<number_of_processes>` with the desired number of MPI processes.

## Implementation Details

The distributed simulator divides the total number of simulations among available MPI processes. Each process runs its allocated simulations independently and then gathers the results to the root process for analysis.

Key features:
- Load balancing across processes
- Efficient communication using MPI gather operations
- Scalable to large numbers of processes

## Extending the Simulator

To add new simulation parameters or analysis methods:

1. Modify the `GameResult` struct to include new data fields.
2. Update the `run_simulations` method to include new parameters.
3. Extend the `analyze_results` method to perform additional analysis on the gathered results.



# Dice Game Simulator

This component provides a flexible and efficient simulation framework for the dice game.

## Features

- Multi-round game simulation
- Customizable player strategies
- Parallel execution of multiple games
- Detailed statistics collection
- JSON output for easy analysis

## Usage

```rust
use dice_game_simulator::{simulate_game, CompositeStrategy, StrategyComponent};

let strategy_a = CompositeStrategy::new(&[StrategyComponent::HighestValue, StrategyComponent::Adaptive]);
let strategy_b = CompositeStrategy::new(&[StrategyComponent::BalancedValue, StrategyComponent::ScoreManagement]);

let (score_a, score_b) = simulate_game(&strategy_a, &strategy_b);
```

## Available Strategies

- Random
- HighestValue
- HighestProbability
- BalancedValue
- Adaptive
- LookAhead
- ScoreManagement

## Running Tournaments

Use the `run_tournament` function to compare multiple strategies:

```rust
let strategies = vec![
    CompositeStrategy::new(&[StrategyComponent::Random]),
    CompositeStrategy::new(&[StrategyComponent::HighestValue]),
    // Add more strategies...
];

let results = run_tournament(&strategies, 10000);
```

## Performance Optimization

For large-scale simulations, use the `UltraOptimizedGame` struct:

```rust
let mut game = UltraOptimizedGame::new(seed);
game.play();
game.print_results();
```

## Extending the Simulator

To add new strategies:

1. Add a new variant to the `StrategyComponent` enum
2. Implement the strategy logic in the `choose_move` function
3. Create new `CompositeStrategy` instances using the new component

## Output

Simulation results are saved in JSON format, including:
- Final scores
- Number of rounds played
- Strategy descriptions
- Win rates



# Interpretation of Simulation Results

After running the simulation and visualizing the results, we can draw several conclusions about the effectiveness of different strategies in both the base game and extended game modes.

## Base Game Analysis

1. **Dominant Strategies**: The LookAhead and Adaptive strategies consistently outperform other strategies, with win rates typically above 60% against most opponents. This suggests that considering future game states and adapting to the current board configuration are crucial for success in the base game.

2. **Balanced Approach**: The BalancedValue strategy performs well overall, often achieving win rates around 55-60% against less sophisticated strategies. This indicates that considering both the probability of rolling certain numbers and the value of bits is a solid approach.

3. **Limitations of Simple Strategies**: While the HighestValue and HighestProbability strategies perform better than Random, they are consistently outperformed by more complex strategies. This suggests that focusing solely on one aspect (value or probability) is suboptimal.

4. **Random Strategy Baseline**: As expected, the Random strategy performs poorly against all other strategies, typically achieving win rates below 40%. This serves as a good baseline to measure the effectiveness of other strategies.

## Extended Game Analysis

1. **First-Player Advantage**: Across all strategy matchups, there appears to be a slight advantage for Player A (trying to turn off all bits). This is likely due to Player A having the first move and the nature of the winning condition.

2. **LookAhead Dominance**: The LookAhead strategy is even more dominant in the extended game, often achieving win rates above 70% against other strategies. This suggests that evaluating future game states is particularly valuable in this game mode.

3. **Adaptive Strategy Performance**: While still strong, the Adaptive strategy's performance in the extended game is not as dominant as in the base game. This might be due to the different nature of the game, where adapting to the current state is less crucial than planning ahead.

4. **Importance of High-Value Bits**: The HighestValue strategy performs better in the extended game compared to the base game, especially when playing as Player B. This indicates that controlling high-value bits is more critical in this game mode.

5. **Probability-Based Strategies**: The HighestProbability strategy performs relatively poorly in the extended game, suggesting that focusing solely on the most likely rolls is not an effective approach in this game mode.

## Overall Conclusions

1. **Complexity Pays Off**: More complex strategies that consider multiple factors (LookAhead, Adaptive) consistently outperform simpler strategies in both game modes.

2. **Game Mode Differences**: The effectiveness of strategies varies between the base game and extended game, highlighting the importance of tailoring strategies to specific game rules.

3. **Balance is Key**: Strategies that balance multiple considerations (value, probability, future states) tend to perform well across different scenarios.

4. **Room for Improvement**: While the LookAhead strategy is dominant, there may be potential for even more sophisticated strategies that combine its strengths with other approaches.

5. **Computational Considerations**: While not measured in this simulation, it's important to note that more complex strategies like LookAhead may require significantly more computation time. In real-time gameplay scenarios, this could be a limiting factor.

These insights can be used to refine game strategies, balance game rules, and potentially develop even more effective approaches to playing the game. Future work could involve developing hybrid strategies that combine the strengths of multiple approaches or exploring machine learning techniques to discover optimal play patterns.