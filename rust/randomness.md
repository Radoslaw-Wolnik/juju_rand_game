## Randomness in Implementations

The quality and performance of random number generation varies across our implementations:

### UltraFastBaseGame and UltraFastGame
- Uses a custom `FastRandom` struct implementing a Xoroshiro128+ algorithm.
- Period: 2^128 - 1
- Statistical Quality: Passes most statistical tests, including BigCrush.
- Speed: Extremely fast, suitable for high-performance simulations.
- Randomness Measure: 8/10 (Very good for games and simulations, not for cryptography)

### FastBaseGame and FastGame
- Uses Rust's `rand::thread_rng()`, which typically implements a cryptographically secure PRNG (often ChaCha20).
- Period: Practically infinite (2^64 or more unique streams, each with a period of at least 2^64)
- Statistical Quality: Passes all known statistical tests.
- Speed: Fast, suitable for most applications including cryptography.
- Randomness Measure: 10/10 (Cryptographically secure)

### SlowBaseGame and SlowGame
- Also uses `rand::thread_rng()`.
- Randomness characteristics identical to FastBaseGame and FastGame.
- Randomness Measure: 10/10

### Seeding
- The ultra-fast implementations use the system time (nanoseconds) for seeding by default.
- Other implementations use Rust's thread-local random number generator, which is automatically seeded with system entropy.
- Entropy of seed: Typically 256 bits for thread_rng() implementations.

### Considerations
- For game simulations, all implementations provide high-quality randomness.
- The ultra-fast implementation trades a small amount of randomness quality for speed.
- Rust's standard library provides strong guarantees about the quality of its random number generation.

Note: The "Randomness Measure" is a simplified 1-10 scale based on period length, statistical test results, and suitability for different applications. It's not a standardized measure but a general guideline.