# Dice Game Simulator (C++ Implementation)

This directory contains the C++ implementation of the dice game simulator, part of the larger Dice Game Analysis Project.

## Implementations

1. **Slow Version (BaseGame)**
   - `src/base_game.cpp`
   - Focuses on readability and detailed logging.
   - Suitable for debugging and understanding game flow.

2. **Fast Version (UltraFastGame)**
   - `src/ultra_fast_game.cpp`
   - Uses atomic operations and a custom fast random number generator.
   - Suitable for high-performance simulations.

3. **Cheaty - Balanced Probability Version (BalancedProbabilityGame)**
   - `src/balanced_probability_game.cpp`
   - Uses a discrete distribution for more accurate dice roll probabilities.
   - Useful for statistical analysis.

4. **Ultra-Optimized Version (UltraOptimizedBaseGame and UltraOptimizedExtendedGame)**
   - `src/ultra_optimized_base_game.cpp` and `src/ultra_optimized_extended_game.cpp`
   - Uses SIMD instructions, cache-friendly data layouts, and extreme optimizations.
   - Suitable for maximum performance in large-scale simulations.


## Building

From the project root:

```bash
cd cpp
mkdir build && cd build
cmake ..
make
```
This will build all versions of the game.

## Running
After building, you can run each version:

``` bash
./base_game
./ultra_fast_game
./balanced_probability_game
```

## Randomness
The quality and performance of random number generation varies across our implementations:

### BaseGame

 - Uses C++'s std::mt19937 (Mersenne Twister) from the <random> library.
 - Period: 2^19937 - 1 (astronomically large)
 - Statistical Quality: Passes all tests in TestU01's BigCrush suite.
 - Speed: Very fast, suitable for most applications.
 - Randomness Measure: 9/10 (Excellent for most purposes, including some scientific simulations)

### UltraFastGame

 - Uses a custom FastRandom class implementing a Xorshift algorithm.
 - Period: 2^64 - 1
 - Statistical Quality: Passes basic statistical tests, but may fail more stringent ones.
 - Speed: Extremely fast, suitable for high-performance simulations.
 - Randomness Measure: 6/10 (Good for games, not for cryptography)

### BalancedProbabilityGame

 - Uses std::discrete_distribution with weights matching the probabilities of dice sums.
 - Underlying RNG: std::mt19937
 - Statistical Quality: Matches the true probabilities of rolling two dice.
 - Speed: Fast, but slightly slower than the other implementations due to additional computation.
 - Randomness Measure: 10/10 (Perfectly matches the probabilities of the physical game)

### UltraOptimizedGame
- Uses a custom Xorshift128+ algorithm for random number generation.
- Period: 2^128 - 1
- Statistical Quality: Passes most statistical tests, including BigCrush.
- Speed: Extremely fast, optimized for SIMD operations.
- Randomness Measure: 8/10 (Very good for games and simulations, not for cryptography)


### Seeding

All implementations use std::random_device for seeding by default.
Entropy of seed: Up to 32 bits on most platforms, potentially more on some systems.

### Considerations

For game simulations, all implementations provide sufficient randomness.
The ultra-fast implementation trades some randomness quality for speed.
If cryptographic-quality randomness is required, consider using a dedicated cryptographic library.

Note: The "Randomness Measure" is a simplified 1-10 scale based on period length, statistical test results, and suitability for different applications. It's not a standardized measure but a general guideline.

## Testing
To run the tests:
``` bash
cd build
ctest
```

## Performance Comparison
To run a performance comparison between all versions:
``` bash
./performance_comparison
```

This will output timing results for each implementation.
Integration with Simulation Framework
The C++ implementations can be integrated with the simulation framework. See simulation/README.md for details on how to use these C++ versions in larger-scale simulations.
Contributing
If you're adding a new implementation or optimizing an existing one:

Add your new files to src/
Update CMakeLists.txt to include your new files
Add appropriate tests in tests/
Update this README with details about your implementation

Please ensure your code follows the project's coding standards and passes all existing tests.

This README provides an overview of the C++ implementation, including details about building, running, and testing the code. The section on randomness explains the different approaches used in each game version, their properties, and considerations for use.

For the game implementations, you would need to update the `UltraFastGame` and `BalancedProbabilityGame` classes to match the descriptions in the README. The `BaseGame` class can remain as previously implemented, using `std::mt19937` for its random number generation.



To use the distributed simulator, you'll need to compile with MPI support and run using mpirun or mpiexec. For example:
mpic++ -std=c++17 -O3 *.cpp -o dice_game_simulator
mpirun -np 4 ./dice_game_simulator