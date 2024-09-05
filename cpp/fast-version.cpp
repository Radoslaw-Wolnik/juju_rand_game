#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <array>
#include <algorithm>

const int ROUNDS = 5;

class FastBaseGame {
private:
    uint16_t board;
    std::array<std::vector<uint16_t>, 13> subsetCache;
    std::array<int, 2> scores;
    std::mt19937 rng;

    void initSubsetCache() {
        for (int i = 2; i <= 12; ++i) {
            for (int j = 1; j < (1 << 12); ++j) {
                if (__builtin_popcount(j) <= i && (j & ((1 << i) - 1)) == j) {
                    int sum = 0;
                    for (int k = 0; k < 12; ++k) {
                        if (j & (1 << k)) sum += k + 1;
                    }
                    if (sum == i) subsetCache[i].push_back(j);
                }
            }
        }
    }

public:
    FastBaseGame(unsigned seed = std::random_device{}())
        : board(0xFFF), scores{0, 0}, rng(seed) {
        initSubsetCache();
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
            int roll = rollDice();
            std::vector<uint16_t> possibleMoves = generatePossibleMoves(roll);
            if (possibleMoves.empty()) {
                return __builtin_popcount(board);
            }

            uint16_t chosenMove = chooseBestMove(possibleMoves);
            board &= ~chosenMove;

            if (board == 0) {
                return 0;
            }
        }
    }

    int rollDice() {
        std::uniform_int_distribution<> dist(1, 6);
        return dist(rng) + dist(rng);
    }

    std::vector<uint16_t> generatePossibleMoves(int roll) {
        std::vector<uint16_t> moves;
        if (board & (1 << (roll - 1))) {
            moves.push_back(1 << (roll - 1));
        }
        for (uint16_t subset : subsetCache[roll]) {
            if ((board & subset) == subset) {
                moves.push_back(subset);
            }
        }
        return moves;
    }

    uint16_t chooseBestMove(const std::vector<uint16_t>& possibleMoves) {
        return *std::max_element(possibleMoves.begin(), possibleMoves.end());
    }
};