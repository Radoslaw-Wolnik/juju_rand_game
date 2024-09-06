# Dice Game Simulation Framework

This directory contains the simulation framework for the Dice Game Analysis project. It provides tools for running large-scale simulations of the game using various strategies and analysis techniques.

## Components

1. **Optimized Simulator**: A high-performance core simulator for running individual games.
2. **Distributed Simulator**: A parallelized simulator using MPI for running simulations across multiple processes or machines.
3. **Comprehensive Simulator**: A tool for running extensive simulations comparing multiple strategies.

## Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler
- MPI library (for distributed simulations)
- nlohmann/json library (for JSON output)

## Building

From the `simulation` directory:

```bash
mkdir build && cd build
cmake ..
make
```

This will build three executables:
- `optimized_simulator`
- `distributed_simulator`
- `comprehensive_simulator`

## Usage

### Optimized Simulator

The optimized simulator is primarily used as a library by other components. You can use it in your own C++ code like this:

```cpp
#include "optimized_simulator.hpp"

OptimizedSimulator simulator(seed);
simulator.addStrategy(std::make_shared<RandomStrategy>());
simulator.addStrategy(std::make_shared<HighestValueStrategy>());

auto result = simulator.runBaseGameSimulation(0, 1);
```

### Distributed Simulator

To run distributed simulations:

```bash
mpirun -np 4 ./distributed_simulator
```

This will run simulations across 4 processes. Adjust the `-np` value based on your available resources.

### Comprehensive Simulator

To run comprehensive simulations comparing all implemented strategies:

```bash
./comprehensive_simulator
```

This will run a large number of games for each strategy matchup and save the results to `simulation_results.json`.

## Extending the Framework

### Adding New Strategies

1. Implement your strategy in the `cpp/include` and `cpp/src` directories.
2. Update `comprehensive_simulator.cpp` to include your new strategy in the simulations.

### Customizing Simulations

You can modify `comprehensive_simulator.cpp` to change the number of games, types of analysis performed, or output format.

## Output

The comprehensive simulator outputs a JSON file with the following structure:

```json
{
  "StrategyA_vs_StrategyB": {
    "base_game": {
      "player_a_wins": 5000,
      "player_b_wins": 5000,
      "player_a_avg_score": 23.5,
      "player_b_avg_score": 24.1
    },
    "extended_game": {
      "player_a_wins": 5100,
      "player_b_wins": 4900
    },
    "avg_rounds": 8.3
  },
  // ... more strategy matchups ...
}
```

## Troubleshooting

- If you encounter MPI-related errors, ensure your MPI library is correctly installed and configured.
- For performance issues, consider reducing the number of simulations or using the distributed simulator on a larger cluster.

For more detailed information on the game rules and strategy implementations, please refer to the main project README.