#include <iostream>
#include <vector>
#include <random>
#include <bitset>
#include <array>
#include <algorithm>
#include <iomanip>

class InteractiveExtendedGame {
private:
    std::bitset<12> board;
    std::array<std::vector<int>, 13> subsetCache;
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
    InteractiveExtendedGame(unsigned seed = std::random_device{}())
        : board(0xFFF), gen(seed), dist(1, 6)
    {
        initSubsetCache();
    }

    void play() {
        std::cout << "Game starts. Initial board state:" << std::endl;
        displayBoard();
        
        bool playerATurn = true;
        while (true) {
            playerTurn(playerATurn);
            
            if (board.none()) {
                std::cout << "Player A wins!" << std::endl;
                break;
            } else if (board.all()) {
                std::cout << "Player B wins!" << std::endl;
                break;
            }
            
            playerATurn = !playerATurn;
        }
    }

    void playerTurn(bool isPlayerA) {
        std::cout << "Player " << (isPlayerA ? "A" : "B") << "'s turn. Press Enter to roll the dice...";
        std::cin.ignore();
        std::cin.get();

        int roll = rollTwoDice();
        std::cout << "You rolled: " << roll << std::endl;

        uint16_t mask = 1 << (roll - 1);
        
        if (isPlayerA) {
            if (board[roll - 1]) {
                std::cout << "Do you want to turn off bit " << roll << "? (y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    board[roll - 1] = 0;
                    std::cout << "Turned off bit " << roll << std::endl;
                } else {
                    std::cout << "Searching for subset..." << std::endl;
                    for (int num : subsetCache[roll]) {
                        if (board[num - 1]) {
                            std::cout << "Do you want to turn off bit " << num << "? (y/n): ";
                            std::cin >> choice;
                            if (choice == 'y' || choice == 'Y') {
                                board[num - 1] = 0;
                                std::cout << "Turned off bit " << num << std::endl;
                                break;
                            }
                        }
                    }
                }
            } else {
                std::cout << "Bit " << roll << " is already off. No valid moves." << std::endl;
            }
        } else {
            if (!board[roll - 1]) {
                std::cout << "Do you want to turn on bit " << roll << "? (y/n): ";
                char choice;
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    board[roll - 1] = 1;
                    std::cout << "Turned on bit " << roll << std::endl;
                } else {
                    std::cout << "Searching for subset..." << std::endl;
                    for (int num : subsetCache[roll]) {
                        if (!board[num - 1]) {
                            std::cout << "Do you want to turn on bit " << num << "? (y/n): ";
                            std::cin >> choice;
                            if (choice == 'y' || choice == 'Y') {
                                board[num - 1] = 1;
                                std::cout << "Turned on bit " << num << std::endl;
                                break;
                            }
                        }
                    }
                }
            } else {
                std::cout << "Bit " << roll << " is already on. No valid moves." << std::endl;
            }
        }

        std::cout << "Current board:" << std::endl;
        displayBoard();
    }
};