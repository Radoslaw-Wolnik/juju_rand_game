# Dice Game Simulator (Rust Implementation)

This directory contains the Rust implementation of the dice game simulator, part of the larger Dice Game Analysis Project.

## Features

- Composite strategy system allowing for complex decision-making processes
- Multiple optimization levels, including an ultra-optimized version
- Integration with the project's simulation framework
- Comprehensive test suite

## Structure

- `src/lib.rs`: Main library file
- `src/game.rs`: Core game logic
- `src/strategies.rs`: Implementation of various game strategies
- `src/ultra_optimized.rs`: Ultra-optimized game implementation
- `tests/`: Directory containing all unit and integration tests

## Building

From the project root:

```bash
cd rust
cargo build --release
```

## Running

To run the basic game simulation:

```bash
cargo run --release
```

For the ultra-optimized version, ensure you have the following in your `Cargo.toml`:

```toml
[profile.release]
lto = "fat"
codegen-units = 1
panic = "abort"
```

Then run:

```bash
cargo run --release --bin ultra_optimized
```

## Testing

To run the test suite:

```bash
cargo test
```

## Benchmarking

To run benchmarks:

```bash
cargo bench
```

## Available Strategies

- Random
- HighestValue
- HighestProbability
- BalancedValue
- Adaptive
- LookAhead
- ScoreManagement

## Integration with Simulation Framework

The Rust implementation is designed to work seamlessly with the project's simulation framework. See `simulation/README.md` for details on how to use these Rust implementations in larger-scale simulations.

## Contributing

If you're adding a new strategy or optimizing an existing one:

1. Add your new code to the appropriate file in `src/`
2. Implement tests for your new code in `tests/`
3. Update this README with details about your implementation
4. Ensure your code passes `cargo clippy` without warnings
5. Run `cargo fmt` before committing

Please make sure your code follows Rust best practices and passes all existing tests.




# Dice Game (Rust Implementation)

This project implements a dice game with two modes and three speed variants in Rust.

## Game Rules

### Base Game
- Players take turns trying to turn off bits on a 12-bit board.
- Each turn, a player rolls two dice and attempts to turn off the corresponding bit.
- If the bit is already off, they can turn off a combination of smaller bits.
- The turn continues until no move is possible.
- Score is the number of bits left on. Lower score is better.
- Game is played for 5 rounds. Player with the lower total score wins.

### Extra Game
- Similar to base game, but players alternate turns.
- Player A tries to turn bits off, Player B tries to turn them on.
- Game ends when all bits are off (Player A wins) or on (Player B wins).

## Game Modes

1. **Base Game**: Implements the base game rules.
2. **Extra Game**: Implements the extra game rules.

## Speed Variants

For each game mode, there are three speed variants:

1. **Ultra-fast**: Optimized for speed with minimal output. Best for large-scale simulations.
2. **Fast**: Balanced between speed and readability. Suitable for general use.
3. **Slow**: Provides detailed output to a file for better analysis of game progress.

## How to Run

1. Ensure you have Rust installed (https://www.rust-lang.org/tools/install)
2. Clone the repository
3. Navigate to the project directory
4. Run the game with: `cargo run --release`
5. Follow the on-screen prompts to choose the game mode and speed variant

## Project Structure

- `src/main.rs`: Contains the main function and user interface.
- `src/ultra_fast_base_game.rs`: Implementation of the ultra-fast base game.
- `src/fast_base_game.rs`: Implementation of the fast base game.
- `src/slow_base_game.rs`: Implementation of the slow base game with detailed output to file.
- `src/ultra_fast_game.rs`: Implementation of the ultra-fast extra game.
- `src/fast_game.rs`: Implementation of the fast extra game.
- `src/slow_game.rs`: Implementation of the slow extra game with detailed output to file.

## Performance Considerations

- The ultra-fast variants use optimized data structures and algorithms for maximum speed.
- The fast variants balance between speed and code readability.
- The slow variants prioritize detailed output over speed, useful for debugging and understanding game flow.

## Future Improvements

- Add options for custom game settings (e.g., number of rounds, board size).
- Implement AI players for single-player mode.
- Create a graphical user interface for a more interactive experience.