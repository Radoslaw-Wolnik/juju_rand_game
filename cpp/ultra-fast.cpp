#include <iostream>
#include <array>
#include <random>
#include <bitset>
#include <algorithm>
#include <atomic>
#include <thread>

const int ROUNDS = 5;

class UltraFastBaseGame {
private:
    std::atomic<uint16_t> board;
    std::array<uint16_t, 13> subsetCache;
    std::array<int, 2> scores;

    class FastRandom {
    private:
        uint64_t state;
    public:
        explicit FastRandom(uint64_t seed) : state(seed) {}
        
        uint64_t next() {
            state ^= state >> 12;
            state ^= state << 25;
            state ^= state >> 27;
            return state * 0x2545F4914F6CDD1DULL;
        }

        int rollDice() {
            return (next() % 6) + 1 + (next() % 6) + 1;
        }
    };

    void initSubsetCache() {
        for (int i = 2; i <= 12; ++i) {
            subsetCache[i] = (1 << i) - 1;
        }
    }

public:
    UltraFastBaseGame(uint64_t seedA = 0, uint64_t seedB = 0) 
        : board(0xFFF), scores{0, 0} {
        initSubsetCache();
    }

    void play() {
        for (int round = 0; round < ROUNDS; ++round) {
            for (int player = 0; player < 2; ++player) {
                board.store(0xFFF, std::memory_order_relaxed);
                int turnScore = playerTurn(player);
                scores[player] += turnScore;
            }
        }

        std::cout << "Game over. Final scores: Player A = " << scores[0] 
                  << ", Player B = " << scores[1] << std::endl;
        std::cout << "Winner: Player " << (scores[0] < scores[1] ? "A" : "B") << std::endl;
    }

    int playerTurn(int player) {
        FastRandom rng(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        while (true) {
            int roll = rng.rollDice();
            uint16_t mask = 1 << (roll - 1);
            uint16_t subset = subsetCache[roll];

            uint16_t oldBoard, newBoard;
            bool success = false;

            do {
                oldBoard = board.load(std::memory_order_relaxed);
                if (oldBoard & mask) {
                    newBoard = oldBoard & ~mask;
                    success = true;
                } else if ((oldBoard & subset) == subset) {
                    newBoard = oldBoard & ~subset;
                    success = true;
                } else {
                    return __builtin_popcount(oldBoard);
                }
            } while (!board.compare_exchange_weak(oldBoard, newBoard, std::memory_order_release, std::memory_order_relaxed));

            if (newBoard == 0) {
                return 0;
            }
        }
    }
};