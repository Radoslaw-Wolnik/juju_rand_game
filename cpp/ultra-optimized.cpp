#include <iostream>
#include <array>
#include <cstdint>
#include <random>
#include <algorithm>
#include <bitset>
#include <immintrin.h>  // For SIMD instructions

constexpr int ROUNDS = 5;
constexpr int NUM_BITS = 12;

class UltraOptimizedGame {
private:
    alignas(64) std::array<uint16_t, 2> boards;
    alignas(64) std::array<uint32_t, 2> scores;
    alignas(64) std::array<uint16_t, 13> subsetCache;
    alignas(64) std::array<uint64_t, 4> xorshiftState;

    static constexpr uint16_t FULL_BOARD = 0xFFF;

    void initSubsetCache() {
        for (int i = 2; i <= 12; ++i) {
            subsetCache[i] = (1 << i) - 1;
        }
    }

    uint64_t xorshift128plus() {
        uint64_t x = xorshiftState[0];
        uint64_t const y = xorshiftState[1];
        xorshiftState[0] = xorshiftState[2];
        xorshiftState[1] = xorshiftState[3];
        x ^= x << 23;
        x ^= x >> 17;
        x ^= y ^ (y >> 26);
        xorshiftState[2] = x;
        xorshiftState[3] = y;
        return x + y;
    }

    uint8_t rollDice() {
        return ((xorshift128plus() & 0x7) % 6) + ((xorshift128plus() & 0x38) % 6) + 2;
    }

    uint16_t generateMove(uint16_t board, uint8_t roll) {
        uint16_t move = board & (1 << (roll - 1));
        if (move) return move;

        uint16_t subset = subsetCache[roll] & board;
        return subset ? _blsi_u32(subset) : 0;  // Use BLSI instruction to get lowest set bit
    }

    uint16_t playTurn(uint16_t& board) {
        uint16_t initial_board = board;
        while (board) {
            uint8_t roll = rollDice();
            uint16_t move = generateMove(board, roll);
            if (!move) break;
            board &= ~move;
        }
        return _mm_popcnt_u32(initial_board) - _mm_popcnt_u32(board);  // Use POPCNT for bit counting
    }

public:
    UltraOptimizedGame(uint64_t seed = std::random_device{}()) {
        initSubsetCache();
        std::seed_seq seq{seed};
        seq.generate(xorshiftState.begin(), xorshiftState.end());
    }

    void play() {
        for (int round = 0; round < ROUNDS; ++round) {
            for (int player = 0; player < 2; ++player) {
                boards[player] = FULL_BOARD;
                scores[player] += playTurn(boards[player]);
            }
        }
    }

    void printResults() {
        std::cout << "Final scores: Player A = " << scores[0] 
                  << ", Player B = " << scores[1] << std::endl;
        std::cout << "Winner: Player " << (scores[0] < scores[1] ? "A" : "B") << std::endl;
    }
};

int main() {
    UltraOptimizedGame game;
    game.play();
    game.printResults();
    return 0;
}