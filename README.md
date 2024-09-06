# Dice Game Analysis Project

This project provides a comprehensive framework for analyzing a dice-based board game through simulation, strategy evaluation, and performance optimization. It includes implementations in both C++ and Rust, along with Python scripts for visualization and a web interface.

## Table of Contents

1. [Project Structure](#project-structure)
2. [Game Rules](#game-rules)
3. [Implementations](#implementations)
4. [Building and Running](#building-and-running)
5. [Strategies](#strategies)
6. [Simulation](#simulation)
7. [Machine Learning](#machine-learning)
8. [Python Scripts](#python-scripts)
9. [Web Interface](#web-interface)
10. [Contributing](#contributing)
11. [License](#license)

## Project Structure

```
dice-game-analysis/
├── cpp/
│   ├── src/
│   ├── include/
│   ├── ml/
│   ├── simulation/
│   └── CMakeLists.txt
├── rust/
│   ├── src/
│   └── Cargo.toml
├── python/
│   ├── cli.py
│   ├── app.py
│   ├── tournament.py
│   └── visualizations.py
├── common/
│   └── strategy_interface.hpp
├── templates/
│   └── index.html
├── requirements.txt
└── README.md
```

## Game Rules

The game is played on a 12-bit board with two variants:

1. **Base Game**: Players try to turn off as many bits as possible in 5 rounds. The player with the lower total score wins.
2. **Extended Game**: One player tries to turn off all bits, while the other tries to turn them all on. The first to achieve their goal wins.

## Implementations

- C++: Provides high-performance simulations with various optimization levels.
- Rust: Offers a clean, safe implementation with good performance.
- Python: Used for visualization, web interface, and machine learning integration.

## Building and Running

### C++

```bash
cd cpp
mkdir build && cd build
cmake ..
make
./dice_game_simulator
```

### Rust

```bash
cd rust
cargo run --release
```

### Python

```bash
pip install -r requirements.txt
python python/cli.py
```

## Strategies

1. Random
2. HighestValue
3. HighestProbability
4. BalancedValue
5. Adaptive
6. LookAhead
7. ScoreManagement
8. RiskAverse
9. Aggressive
10. PatternRecognition

## Simulation

The project supports various simulation modes:

- Single-threaded
- Multi-threaded
- Distributed (using MPI)

To run a distributed simulation:

```bash
mpirun -np 4 ./dice_game_simulator
```

## Machine Learning

The project includes a Deep Q-Network (DQN) agent for reinforcement learning:

```bash
python python/train_dqn.py
```

## Python Scripts

- `cli.py`: Command-line interface for running simulations
- `tournament.py`: Runs a tournament between different strategies
- `visualizations.py`: Creates visualizations of game states and strategy performance

## Web Interface

A Flask-based web interface is provided for interactive gameplay and analysis:

```bash
python python/app.py
```

Then open a web browser and navigate to `http://localhost:5000`.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.