#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <array>
#include <algorithm>
#include <iomanip>

class InteractiveBaseGame {
private:
    std::bitset<12> board;
    std::array<std::vector<int>, 13> subsetCache;
    std::array<int, 2> scores;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;

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
    InteractiveBaseGame(unsigned seed = std::random_device{}())
        : board(0xFFF), scores{0, 0}, gen(seed), dist(1, 6)
    {
        initSubsetCache();
    }

    void play() {
        int rounds;
        std::cout << "Enter the number of rounds to play: ";
        std::cin >> rounds;

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
    }

    int playerTurn(int player) {
        std::cout << "Player " << (player == 0 ? "A" : "B") << "'s turn begins. Initial board:" << std::endl;
        displayBoard();
        
        while (true) {
            std::cout << "Press Enter to roll the dice...";
            std::cin.ignore();
            std::cin.get();

            int roll = rollTwoDice();
            std::cout << "You rolled: " << roll << std::endl;

            if (board[roll - 1]) {
                std::cout << "Do you want to turn off bit " << roll << "? (y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    board[roll - 1] = 0;
                    std::cout << "Turned off bit " << roll << std::endl;
                } else {
                    std::cout << "Searching for subset..." << std::endl;
                    bool subsetFound = false;
                    for (int num : subsetCache[roll]) {
                        if (board[num - 1]) {
                            std::cout << "Do you want to turn off bit " << num << "? (y/n): ";
                            std::cin >> choice;
                            if (choice == 'y' || choice == 'Y') {
                                board[num - 1] = 0;
                                subsetFound = true;
                                std::cout << "Turned off bit " << num << std::endl;
                                break;
                            }
                        }
                    }
                    if (!subsetFound) {
                        int score = board.count();
                        std::cout << "No valid moves. Turn ends with score: " << score << std::endl;
                        std::cout << "Final board state:" << std::endl;
                        displayBoard();
                        return score;
                    }
                }
            } else {
                std::cout << "Bit " << roll << " is already off. Searching for subset..." << std::endl;
                bool subsetFound = false;
                for (int num : subsetCache[roll]) {
                    if (board[num - 1]) {
                        std::cout << "Do you want to turn off bit " << num << "? (y/n): ";
                        char choice;
                        std::cin >> choice;
                        if (choice == 'y' || choice == 'Y') {
                            board[num - 1] = 0;
                            subsetFound = true;
                            std::cout << "Turned off bit " << num << std::endl;
                            break;
                        }
                    }
                }
                if (!subsetFound) {
                    int score = board.count();
                    std::cout << "No valid moves. Turn ends with score: " << score << std::endl;
                    std::cout << "Final board state:" << std::endl;
                    displayBoard();
                    return score;
                }
            }

            std::cout << "Current board:" << std::endl;
            displayBoard();

            if (board.none()) {
                std::cout << "All bits turned off! Perfect score of 0." << std::endl;
                return 0;
            }
        }
    }
};