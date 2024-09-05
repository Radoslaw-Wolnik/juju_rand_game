// ProperRandomBaseGame.hpp
#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <array>
#include <algorithm>

class ProperRandomBaseGame {
private:
    std::bitset<12> board;
    std::array<std::vector<int>, 13> subsetCache;
    std::array<int, 2> scores;
    std::random_device rd;
    std::mt19937_64 gen;
    std::uniform_int_distribution<> dice_dist;

    void initSubsetCache() {
        for (int i = 2; i <= 12; ++i) {
            for (int j = 1; j < i; ++j) {
                if ((1 << (j - 1)) & ((1 << i) - 1)) {
                    subsetCache[i].push_back(j);
                }
            }
        }
    }

    int rollTwoDice() {
        return dice_dist(gen) + dice_dist(gen);
    }

public:
    ProperRandomBaseGame()
        : board(0xFFF), scores{0, 0}, gen(rd()), dice_dist(1, 6) {
        initSubsetCache();
    }

    void play() {
        for (int round = 0; round < 5; ++round) {
            std::cout << "Round " << round + 1 << " begins:" << std::endl;
            for (int player = 0; player < 2; ++player) {
                board = 0xFFF;
                int turnScore = playerTurn(player);
                scores[player] += turnScore;
                std::cout << "Player " << (player == 0 ? "A" : "B") << " round score: " << turnScore << std::endl;
                std::cout << "Current total: Player A = " << scores[0] << ", Player B = " << scores[1] << std::endl << std::endl;
            }
        }

        std::cout << "Game over. Final scores: Player A = " << scores[0] 
                  << ", Player B = " << scores[1] << std::endl;
        std::cout << "Winner: Player " << (scores[0] < scores[1] ? "A" : "B") << std::endl;
    }

    int playerTurn(int player) {
        std::cout << "Player " << (player == 0 ? "A" : "B") << "'s turn begins. Initial board: " << board << std::endl;
        while (true) {
            int roll = rollTwoDice();
            std::cout << "  Rolled: " << roll << std::endl;

            if (board[roll - 1]) {
                board[roll - 1] = 0;
                std::cout << "  Turned off bit " << roll << std::endl;
            } else {
                bool subsetFound = false;
                for (int num : subsetCache[roll]) {
                    if (board[num - 1]) {
                        board[num - 1] = 0;
                        subsetFound = true;
                        std::cout << "  Turned off bit " << num << " (subset of " << roll << ")" << std::endl;
                    }
                }
                if (!subsetFound) {
                    int score = board.count();
                    std::cout << "  No valid moves. Turn ends with score: " << score << std::endl;
                    std::cout << "  Final board state: " << board << std::endl;
                    return score;
                }
            }

            std::cout << "  Current board: " << board << std::endl;
            if (board.none()) {
                std::cout << "  All bits turned off! Perfect score of 0." << std::endl;
                return 0;
            }
        }
    }
};