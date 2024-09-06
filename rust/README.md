# Dice Game Simulator

This project provides a comprehensive framework for simulating and analyzing strategies for a dice-based board game. It includes multiple simulation modes, various optimization levels, and supports both local and distributed computing.

## Game Rules

The game is played on a 12-bit board with two variants:

1. **Base Game**: Players try to turn off as many bits as possible in 5 rounds. The player with the lower total score wins.
2. **Extended Game**: One player tries to turn off all bits, while the other tries to turn them all on. The first to achieve their goal wins.

## Project Structure

- `src/`
  - `main.rs`: Contains the command-line interface and main entry point
  - `lib.rs`: Exports the main components of the simulator
  - `game.rs`: Implements the game logic
  - `strategies.rs`: Implements the different game strategies
  - `simulation.rs`: Handles the simulation logic and delegates to specific simulators
  - `multithreaded_simulator.rs`: Implements multithreaded simulation
  - `parallel_simulator.rs`: Implements parallel simulation using rayon
  - `distributed_simulator.rs`: Implements distributed simulation using MPI
- `python/`
  - `app.py`: Flask application for web interface
  - `tournament.py`: Python implementation of tournament logic (for web interface)
  - `visualizations.py`: Python script with visualization functions
- `templates/`
  - `index.html`: HTML template for the web interface
- `Cargo.toml`: Rust project configuration
- `requirements.txt`: Python dependencies

## Installation

1. Ensure you have Rust and Python installed
2. Clone this repository
3. Navigate to the project directory
4. Install Rust dependencies:
   ```
   cargo build --release
   ```
5. Install Python dependencies:
   ```
   pip install -r requirements.txt
   ```

## Usage

### Command-Line Interface

The simulator can be run from the command line with various options:

```
cargo run --release -- [OPTIONS]
```

Options:
- `-m, --mode <MODE>`: Choose between 'base' and 'extended' game modes
- `-o, --optimization <LEVEL>`: Set the optimization level ('standard', 'fast', or 'ultra')
- `-s, --strategies <STRATEGIES>`: Specify which strategies to include in the simulation (comma-separated)
- `-n, --num-games <NUM>`: Set the number of games to simulate
- `--output <FILE>`: Specify the output file for results (default is 'tournament_results.json')
- `--simulator <TYPE>`: Choose the simulator type ('multithreaded', 'parallel', or 'distributed')

Example usage:
```
cargo run --release -- -m base -o fast -s "Random,Highest Value,Adaptive Score Management" -n 10000 --simulator parallel
```

### Web Interface

To start the web interface:

1. Run `python python/app.py`
2. Open a web browser and navigate to `http://localhost:5000`

## Simulation Modes and Randomness Levels

The simulator offers different optimization levels, each with its own balance of speed and randomness quality:

### Standard Mode
- Uses Rust's `thread_rng()`, which is typically a cryptographically secure PRNG
- Randomness Measure: 10/10 (Cryptographically secure)
- Suitable for most applications including cryptography
- Slower but highest quality randomness

### Fast Precise Mode
- Uses Rust's `rand::thread_rng()`, which typically implements a cryptographically secure PRNG (often ChaCha20)
- Period: Practically infinite (2^64 or more unique streams, each with a period of at least 2^64)
- Statistical Quality: Passes all known statistical tests
- Speed: Fast, suitable for most applications including cryptography
- Randomness Measure: 10/10 (Cryptographically secure)

### Fast Mode
- Uses a custom fast random number generator (Xoroshiro128+)
- Randomness Measure: 8/10 (Very good for games and simulations, not for cryptography)
- Period: 2^128 - 1
- Passes most statistical tests, including BigCrush
- Significantly faster than Standard mode, with good randomness quality

### Ultra-Optimized Mode
- Uses a highly optimized RNG implementation
- Randomness Measure: 7/10 (Good for high-performance simulations)
- Extremely fast, suitable for large-scale simulations
- May sacrifice some randomness quality for speed

## Implemented Strategies

1. Random: Makes random moves
2. HighestValue: Prioritizes turning off/on the highest value bits
3. HighestProbability: Prioritizes the most probable bits (7, 6, 8, etc.)
4. BalancedValue: Balances between value and probability
5. Adaptive: Changes strategy based on the game state
6. LookAhead: Evaluates potential future states
7. ScoreManagement: Adjusts strategy based on the current score difference
8. RiskAverse: Prefers safer, lower-value moves
9. Aggressive: Always goes for the highest value moves
10. PatternRecognition: Recognizes patterns in game progression

## Simulator Types

1. **Multithreaded**: Uses standard Rust threads for parallelism. Good for simulations on a single multi-core machine.
2. **Parallel**: Uses the Rayon library for efficient local parallelism. Best for high-performance simulations on a single machine.
3. **Distributed**: Uses MPI for distributed computing across multiple machines. Ideal for large-scale simulations using a computer cluster.

## Extending the Project

To add new strategies:
1. Implement the new strategy in `src/strategies.rs`
2. Add the new strategy to the `create_strategies()` function in `src/strategies.rs`
3. Re-run the simulation and analyze the results

To add new simulator types:
1. Create a new file (e.g., `new_simulator.rs`) in the `src/` directory
2. Implement the simulator logic
3. Add the new simulator type to the `SimulatorType` enum in `src/simulation.rs`
4. Update the `run_simulation` function in `src/simulation.rs` to handle the new simulator type

## Analyzing Results

The simulation outputs results in JSON format, which can be visualized using the provided Python scripts. The web interface also offers interactive visualizations of the simulation results.

When analyzing the results, consider:
- Which strategies perform consistently well across different matchups?
- Are there significant differences in strategy effectiveness between the base game and extended game?
- How do more complex strategies (e.g., LookAhead) compare to simpler ones (e.g., HighestValue) in terms of win rate and computational cost?
- How does the choice of optimization level affect the results?

## License

[Specify your chosen license here]

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.