#include "ai_player.hpp"

int AIPlayer::chooseMove(unsigned short board, int roll) {
    // Implement AI logic here
    // This could be a more sophisticated algorithm than the basic strategies
    // For example, it could use a simple heuristic or a pre-trained model
    
    // Simple example (similar to HighestValueStrategy):
    for (int i = roll; i >= 1; --i) {
        if (board & (1 << (i - 1))) {
            return 1 << (i - 1);
        }
    }
    return 0;
}