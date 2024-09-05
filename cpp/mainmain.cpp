// main.cpp
#include "UltraFastBaseGame.hpp"
#include "FastBaseGame.hpp"
#include "SlowBaseGame.hpp"
#include "SlowFileBaseGame.hpp"
#include "ProperRandomBaseGame.hpp"
#include "UltraFastGame.hpp"
#include "FastGame.hpp"
#include "SlowMoGame.hpp"
#include <iostream>
#include <string>
#include <limits>

// ... (previous code remains the same)

int main() {
    while (true) {
        explainRules();

        std::cout << "\nPlease choose a game mode:\n"
                  << "1. Base Game\n"
                  << "2. Extra Game\n"
                  << "3. Exit\n";

        int modeChoice = getIntInput("Enter your choice (1-3): ");

        if (modeChoice == 3) {
            std::cout << "Thanks for playing. Goodbye!\n";
            break;
        }

        if (modeChoice != 1 && modeChoice != 2) {
            std::cout << "Invalid choice. Please try again.\n";
            continue;
        }

        std::cout << "\nChoose an algorithm speed:\n"
                  << "1. Ultra-fast (minimal output)\n"
                  << "2. Fast (balanced speed and output)\n"
                  << "3. Slow (detailed console output)\n"
                  << "4. Slow (detailed file output)\n"
                  << "5. Proper Random (cryptographically secure, may be slower)\n";

        int speedChoice = getIntInput("Enter your choice (1-5): ");

        if (speedChoice < 1 || speedChoice > 5) {
            std::cout << "Invalid choice. Please try again.\n";
            continue;
        }

        if (modeChoice == 1) {  // Base Game
            switch (speedChoice) {
                case 1: {
                    UltraFastBaseGame game;
                    game.play();
                    break;
                }
                case 2: {
                    FastBaseGame game;
                    game.play();
                    break;
                }
                case 3: {
                    SlowBaseGame game;
                    game.play();
                    break;
                }
                case 4: {
                    SlowFileBaseGame game;
                    game.play();
                    break;
                }
                case 5: {
                    ProperRandomBaseGame game;
                    game.play();
                    break;
                }
            }
        } else {  // Extra Game
            switch (speedChoice) {
                case 1: {
                    UltraFastGame game;
                    game.play();
                    break;
                }
                case 2: {
                    FastGame game;
                    game.play();
                    break;
                }
                case 3: {
                    SlowMoGame game;
                    game.play();
                    break;
                }
                case 4: {
                    std::cout << "File output not implemented for Extra Game. Using console output instead.\n";
                    SlowMoGame game;
                    game.play();
                    break;
                }
                case 5: {
                    std::cout << "Proper Random not implemented for Extra Game. Using Fast Game instead.\n";
                    FastGame game;
                    game.play();
                    break;
                }
            }
        }

        // ... (rest of the code remains the same)
    }

    return 0;
}