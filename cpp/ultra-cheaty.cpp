// UltraCheakyFastGame.hpp
#pragma once

#include <iostream>
#include <array>
#include <random>
#include <bitset>

class UltraCheatyFastGame {
private:
    uint16_t board;
    std::array<std::vector<int>, 13> subsetCache;
    std::array<int, 2> scores;
    std::mt19937 gen;
    std::discrete_distribution<> dist;

    void initSubsetCache() {
        for (int i = 2; i <= 12; ++i) {
            for (int j = 1; j < i; ++j) {
                if ((1 << (j - 1)) & ((1 << i) - 1)) {
                    subsetCache[i].push_back(j);
                }
            }
        }
    }

    int rollWeightedDice() {
        return dist(gen) + 2; // +2 because our distribution is 0-indexed
    }

public:
    UltraCheatyFastGame(unsigned seed = std::random_device{}())
        : board(0xFFF), scores{0, 0}, gen(seed),
          dist({1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1}) // Weights for 2 to 12
    {
        initSubsetCache();
    }

    void play() {
        for (int round = 0; round < 5; ++round) {
            for (int player = 0; player < 2; ++player) {
                board = 0xFFF;
                int turnScore = playerTurn(player);
                scores[player] += turnScore;
            }
        }

        std::cout << "Game over. Final scores: Player A = " << scores[0] 
                  << ", Player B = " << scores[1] << std::endl;
        std::cout << "Winner: Player " << (scores[0] < scores[1] ? "A" : "B") << std::endl;
    }

    int playerTurn(int player) {
        while (true) {
            int roll = rollWeightedDice();
            uint16_t mask = 1 << (roll - 1);

            if (board & mask) {
                board &= ~mask;
            } else {
                bool subsetFound = false;
                for (int num : subsetCache[roll]) {
                    if (board & (1 << (num - 1))) {
                        board &= ~(1 << (num - 1));
                        subsetFound = true;
                    }
                }
                if (!subsetFound) {
                    return std::bitset<12>(board).count();
                }
            }

            if (board == 0) {
                return 0;  // Perfect score
            }
        }
    }
};