#include <iostream>
#include <array>
#include <random>
#include <bitset>
#include <algorithm>

const int ROUNDS = 5;

class CheatyBalancedBaseGame {
private:
    uint16_t board;
    std::array<int, 2> scores;
    std::discrete_distribution<> diceDistribution;
    std::mt19937 gen;

    void initDiceDistribution() {
        // Probabilities for sums of two dice (2 to 12)
        std::array<double, 11> probabilities = {1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1};
        diceDistribution = std::discrete_distribution<>(probabilities.begin(), probabilities.end());
    }

public:
    CheatyBalancedBaseGame(unsigned seed = std::random_device{}())
        : board(0xFFF), scores{0, 0}, gen(seed) {
        initDiceDistribution();
    }

    void play() {
        for (int round = 0; round < ROUNDS; ++round) {
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
            int roll = diceDistribution(gen) + 2; // +2 because distribution is 0-indexed
            uint16_t mask = 1 << (roll - 1);

            if (board & mask) {
                board &= ~mask;
            } else {
                // Simple subset sum for demonstration
                for (int i = 1; i < roll; ++i) {
                    if (board & (1 << (i - 1))) {
                        board &= ~(1 << (i - 1));
                        break;
                    }
                }
            }

            if (board == 0) {
                return 0;
            }

            if ((board & ((1 << roll) - 1)) == 0) {
                return __builtin_popcount(board);
            }
        }
    }
};