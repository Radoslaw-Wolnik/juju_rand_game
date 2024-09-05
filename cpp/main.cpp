#include "InteractiveBaseGame.hpp"
#include "InteractiveExtendedGame.hpp"
#include "MultithreadedSimulations.hpp"
// ... (other includes)

int main() {
    while (true) {
        std::cout << "\nChoose a game mode:\n"
                  << "1. Interactive Base Game\n"
                  << "2. Interactive Extended Game\n"
                  << "3. AI Game (Base Rules)\n"
                  << "4. AI Game (Extended Rules)\n"
                  << "5. Run Multithreaded Simulations\n"
                  << "6. Exit\n";

        int choice = getIntInput("Enter your choice (1-6): ");

        switch (choice) {
            case 1: {
                InteractiveBaseGame game;
                game.play();
                break;
            }
            case 2: {
                InteractiveExtendedGame game;
                game.play();
                break;
            }
            case 3: {
                // Implement AI game with base rules
                break;
            }
            case 4: {
                // Implement AI game with extended rules
                break;
            }
            case 5: {
                MultithreadedSimulations simulations;
                simulations.runSimulations(10000, 4);
                break;
            }
            case 6:
                std::cout << "Thanks for playing. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}