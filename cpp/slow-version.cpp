#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <array>
#include <algorithm>
#include <fstream>

const int ROUNDS = 5;

class SlowBaseGame {
private:
    std::bitset<12> board;
    std::array<std::vector<int>, 13> subsetCache;
    std::array<int, 2> scores;
    std::mt19937 rng;
    std::ofstream logFile;

    void initSubsetCache() {
        for (int i = 2; i <= 12; ++i) {
            for (int j = 1; j < i; ++j) {
                if ((1 << (j - 1)) & ((1 << i) - 1)) {
                    subsetCache[i].push_back(j);
                }
            }
        }
    }

public:
    SlowBaseGame(unsigned seed = std::random_device{}())
        : board(0xFFF), scores{0, 0}, rng(seed) {
        initSubsetCache();
        logFile.open("slow_base_game_log.txt");
    }

    ~SlowBaseGame() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void play() {
        for (int round = 0; round < ROUNDS; ++round) {
            logFile << "Round " << round + 1 << " begins:" << std::endl;
            for (int player = 0; player < 2; ++player) {
                board = 0xFFF;
                int turnScore = playerTurn(player);
                scores[player] += turnScore;
                logFile << "Player " << (player == 0 ? "A" : "B") << " round score: " << turnScore << std::endl;
                logFile << "Current total: Player A = " << scores[0] << ", Player B = " << scores[1] << std::endl << std::endl;
            }
        }

        logFile << "Game over. Final scores: Player A = " << scores[0] 
                << ", Player B = " << scores[1] << std::endl;
        logFile << "Winner: Player " << (scores[0] < scores[1] ? "A" : "B") << std::endl;
    }

    int playerTurn(int player) {
        logFile << "Player " << (player == 0 ? "A" : "B") << "'s turn begins. Initial board: " << board << std::endl;
        while (true) {
            int roll = rollDice();
            logFile << "  Rolled: " << roll << std::endl;

            std::vector<int> possibleMoves = generatePossibleMoves(roll);
            if (possibleMoves.empty()) {
                int score = board.count();
                logFile << "  No valid moves. Turn ends with score: " << score << std::endl;
                logFile << "  Final board state: " << board << std::endl;
                return score;
            }

            int chosenMove = chooseBestMove(possibleMoves);
            board &= ~chosenMove;
            logFile << "  Turned off bits: " << std::bitset<12>(chosenMove) << std::endl;
            logFile << "  Current board: " << board << std::endl;

            if (board.none()) {
                logFile << "  All bits turned off! Perfect score of 0." << std::endl;
                return 0;
            }
        }
    }

    int rollDice() {
        std::uniform_int_distribution<> dist(1, 6);
        return dist(rng) + dist(rng);
    }

    std::vector<int> generatePossibleMoves(int roll) {
        std::vector<int> moves;
        if (board[roll - 1]) {
            moves.push_back(1 << (roll - 1));
        }
        // Add subset sum moves
        for (int subset : subsetCache[roll]) {
            if ((board & subset) == subset) { // error
                moves.push_back(subset);
            }
        }
        return moves;
    }

    int chooseBestMove(const std::vector<int>& possibleMoves) {
        // In slow version, we'll just choose the move that turns off the most significant bit
        return *std::max_element(possibleMoves.begin(), possibleMoves.end());
    }
};