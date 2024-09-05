## Randomness in Implementations

The quality and performance of random number generation varies across our implementations:

### UltraFastBaseGame and UltraFastGame
- Uses a custom `FastRandom` class implementing a Xorshift algorithm.
- Period: 2^64 - 1
- Statistical Quality: Passes basic statistical tests, but may fail more stringent ones.
- Speed: Extremely fast, suitable for high-performance simulations.
- Randomness Measure: 6/10 (Good for games, not for cryptography)

### FastBaseGame and FastGame
- Uses C++'s `std::mt19937` (Mersenne Twister) from the `<random>` library.
- Period: 2^19937 - 1 (astronomically large)
- Statistical Quality: Passes all tests in TestU01's BigCrush suite.
- Speed: Very fast, suitable for most applications.
- Randomness Measure: 9/10 (Excellent for most purposes, including some scientific simulations)

### SlowBaseGame, SlowFileBaseGame, and SlowMoGame
- Also uses `std::mt19937`.
- Randomness characteristics identical to FastBaseGame and FastGame.
- Randomness Measure: 9/10

### Seeding
- All implementations use `std::random_device` for seeding by default.
- Entropy of seed: Up to 32 bits on most platforms, potentially more on some systems.

### Considerations
- For game simulations, all implementations provide sufficient randomness.
- The ultra-fast implementation trades some randomness quality for speed.
- If cryptographic-quality randomness is required, consider using a dedicated cryptographic library (randomness measure would be 10/10).

Note: The "Randomness Measure" is a simplified 1-10 scale based on period length, statistical test results, and suitability for different applications. It's not a standardized measure but a general guideline.