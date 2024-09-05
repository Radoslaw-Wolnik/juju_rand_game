# Dice Game Strategy Simulation

This project simulates various strategies for a dice-based board game and compares their effectiveness through a tournament-style competition.

## Game Rules

The game is played on a 12-bit board with two variants:

1. **Base Game**: Players try to turn off as many bits as possible in 5 rounds. The player with the lower total score wins.
2. **Extended Game**: One player tries to turn off all bits, while the other tries to turn them all on. The first to achieve their goal wins.

## Implemented Strategies

1. Random: Makes random moves
2. HighestValue: Prioritizes turning off/on the highest value bits
3. HighestProbability: Prioritizes the most probable bits (7, 6, 8, etc.)
4. BalancedValue: Balances between value and probability
5. Adaptive: Changes strategy based on the game state
6. LookAhead: Evaluates potential future states

## Project Structure

- `src/main.rs`: Contains the main simulation logic and tournament runner
- `src/strategies.rs`: Implements the different game strategies
- `visualize_results.py`: Python script to visualize the simulation results

## Requirements

- Rust (latest stable version)
- Python 3.x (for visualization)
- Python libraries: matplotlib, seaborn, pandas

## How to Run

1. Ensure you have Rust installed (https://www.rust-lang.org/tools/install)
2. Clone this repository
3. Navigate to the project directory
4. Run the simulation:
   ```
   cargo run --release
   ```
5. After the simulation completes, run the visualization script:
   ```
   python visualize_results.py
   ```

## Interpreting Results

The simulation outputs a JSON file (`tournament_results.json`) containing win rates for each strategy matchup in both game modes. The visualization script generates heatmaps to help interpret these results.

When analyzing the results, consider:
- Which strategies perform consistently well across different matchups?
- Are there significant differences in strategy effectiveness between the base game and extended game?
- How do more complex strategies (e.g., LookAhead) compare to simpler ones (e.g., HighestValue) in terms of win rate and computational cost?

## Extending the Project

To add new strategies:
1. Implement the new strategy in `src/strategies.rs`
2. Add the new strategy to the `strategies` array in `src/main.rs`
3. Re-run the simulation and visualization

## License

[Specify your chosen license here]