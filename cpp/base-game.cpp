// ImprovedBaseGame.hpp
#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <array>
#include <algorithm>
#include <iomanip>

class ImprovedBaseGame {
private:
    std::bitset<12> board;
    std::array<std::vector<int>, 13> subsetCache;
    std::array<int, 2> scores;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
    int rounds;
    int turnCount;

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
        return dist(gen) + dist(gen);
    }

    void displayBoard() const {
        std::cout << "+---+---+---+---+---+---+---+---+---+---+---+---+\n";
        std::cout << "| ";
        for (int i = 11; i >= 0; --i) {
            std::cout << (board[i] ? "O" : "X") << " | ";
        }
        std::cout << "\n+---+---+---+---+---+---+---+---+---+---+---+---+\n";
        std::cout << "  12  11  10   9   8   7   6   5   4   3   2   1  \n";
    }

public:
    ImprovedBaseGame(unsigned seed = std::random_device{}(), int numRounds = 5)
        : board(0xFFF), scores{0, 0}, gen(seed), dist(1, 6), rounds(numRounds), turnCount(0)
    {
        initSubsetCache();
    }

    void play() {
        for (int round = 0; round < rounds; ++round) {
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
        std::cout << "Total turns: " << turnCount << std::endl;
        std::cout << "Average turns per round: " << std::fixed << std::setprecision(2) 
                  << static_cast<float>(turnCount) / (rounds * 2) << std::endl;
    }

    int playerTurn(int player) {
        std::cout << "Player " << (player == 0 ? "A" : "B") << "'s turn begins. Initial board:" << std::endl;
        displayBoard();
        int turns = 0;
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
                    std::cout << "  Final board state:" << std::endl;
                    displayBoard();
                    turnCount += turns + 1;
                    return score;
                }
            }

            std::cout << "  Current board:" << std::endl;
            displayBoard();
            turns++;

            if (board.none()) {
                std::cout << "  All bits turned off! Perfect score of 0." << std::endl;
                turnCount += turns + 1;
                return 0;
            }
        }
    }
};