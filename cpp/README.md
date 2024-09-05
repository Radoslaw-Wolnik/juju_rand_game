# Dice Game Simulator (C++ Implementation)

This directory contains the C++ implementation of the dice game simulator, part of the larger Dice Game Analysis Project.

## Implementations

1. **Slow Version (SlowBaseGame)**
   - `src/slow_base_game.cpp`
   - Focuses on readability and detailed logging.
   - Suitable for debugging and understanding game flow.

2. **Fast Version (FastBaseGame)**
   - `src/fast_base_game.cpp`
   - Balances speed and readability.
   - Suitable for most use cases.

3. **Ultra Fast Version (UltraFastBaseGame)**
   - `src/ultra_fast_base_game.cpp`
   - Uses atomic operations and a custom fast random number generator.
   - Suitable for high-performance simulations.

4. **Cheaty - Balanced Probability Version (CheatyBalancedBaseGame)**
   - `src/cheaty_balanced_base_game.cpp`
   - Uses a discrete distribution for more accurate dice roll probabilities.
   - Useful for statistical analysis.

5. **Ultra-Optimized Version (UltraOptimizedGame)**
   - `src/ultra_optimized_game.cpp`
   - Employs SIMD instructions, cache-friendly data layouts, and extreme optimizations.
   - Designed for maximum performance in large-scale simulations.

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

```bash
./slow_base_game
./fast_base_game
./ultra_fast_base_game
./cheaty_balanced_base_game
./ultra_optimized_game
```

## Testing

To run the tests:

```bash
cd build
ctest
```

## Performance Comparison

To run a performance comparison between all versions:

```bash
./performance_comparison
```

This will output timing results for each implementation.

## Integration with Simulation Framework

The C++ implementations can be integrated with the simulation framework. See `simulation/README.md` for details on how to use these C++ versions in larger-scale simulations.

## Contributing

If you're adding a new implementation or optimizing an existing one:

1. Add your new files to `src/`
2. Update `CMakeLists.txt` to include your new files
3. Add appropriate tests in `tests/`
4. Update this README with details about your implementation

Please ensure your code follows the project's coding standards and passes all existing tests.