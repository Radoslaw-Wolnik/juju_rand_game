# Dice Game Analysis Project

This project provides a comprehensive framework for analyzing a dice-based board game through simulation, strategy evaluation, and performance optimization.

## New Features

1. **Unified Strategy Interface**: Common interface for strategies in both C++ and Rust.
2. **Machine Learning Integration**: Reinforcement learning with DQN for strategy optimization.
3. **Parallelization**: Distributed computing for large-scale simulations using MPI.
4. **Visualization Tools**: Python-based tools for visualizing game states, strategy performance, and tournament results.
5. **Dynamic Strategy Adaptation**: Adaptive meta-strategy that learns from performance metrics.
6. **Game Theory Analysis**: Comprehensive analysis including Nash Equilibrium, dominating strategies, and evolutionary stability.
7. **Web Interface**: Flask-based web application for interactive gameplay and analysis.

## Project Structure

```
shut-the-box-game/
│
├── common/
│   ├── strategies/
│   │   ├── strategy_interface.hpp
│   │   ├── strategy_interface.rs
│   │   ├── adaptive_strategy.hpp
│   │   └── adaptive_strategy.rs
│
├── cpp/
│   └── ... (existing C++ implementation)
│
├── rust/
│   └── ... (existing Rust implementation)
│
├── simulation/
│   ├── parallel/
│   │   ├── distributed_simulator.cpp
│   │   └── distributed_simulator.rs
│
├── visualization/
│   ├── game_visualizer.py
│   ├── strategy_analyzer.py
│   └── tournament_visualizer.py
│
├── ml/
│   └── reinforcement/
│       └── dqn_agent.py
│
├── verification/
│   └── game_rules_verification.v
│
├── web/
│   ├── app.py
│   └── templates/
│       └── index.html
│
├── analysis/
│   └── game_theory/
│       └── comprehensive_analysis.py
│
├── .github/
│   └── workflows/
│       ├── ci_cpp.yml
│       └── ci_rust.yml
│
└── README.md (this file)
```

## Installation and Setup

[Provide instructions for setting up the project, including dependencies]

## Usage

### Running Simulations

```bash
# C++ version
cd cpp && make && ./dice_game_simulator

# Rust version
cd rust && cargo run --release

# Distributed simulation
mpirun -np 4 ./distributed_simulator
```

### Web Interface

```bash
cd web
flask run
```

Then open a web browser and navigate to `http://localhost:5000`.

### Visualization

```bash
python visualization/game_visualizer.py
python visualization/strategy_analyzer.py
python visualization/tournament_visualizer.py
```

### Machine Learning

```bash
python ml/reinforcement/train_dqn.py
```

### Game Theory Analysis

```bash
python analysis/game_theory/comprehensive_analysis.py
```

## Documentation

### Unified Strategy Interface

The unified strategy interface allows for consistent strategy implementation across C++ and Rust. It defines the following methods:

- `choose_move(game_state)`: Selects the next move based on the current game state.
- `name()`: Returns the name of the strategy.
- `description()`: Provides a brief description of the strategy.

### Parallelization

The distributed simulator uses MPI to parallelize game simulations across multiple processes. This allows for significantly faster large-scale simulations.

### Dynamic Strategy Adaptation

The adaptive strategy maintains a collection of other strategies and dynamically adjusts the probability of choosing each strategy based on their performance. It considers both win rate and average score, adapting every 100 games.

### Game Theory Analysis

The game theory analyzer provides the following functionalities:

- Nash Equilibrium calculation
- Identification of dominating strategies
- Best response strategy calculation
- Visualization of payoff matrices
- Evolutionary stable strategy simulation

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the [specify your license] - see the [LICENSE.md](LICENSE.md) file for details.