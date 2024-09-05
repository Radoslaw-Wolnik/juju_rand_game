class AIPlayer {
public:
    static bool makeMove(std::bitset<12>& board, int roll, bool isPlayerA) {
        uint16_t mask = 1 << (roll - 1);
        
        if (isPlayerA) {
            if (board[roll - 1]) {
                board[roll - 1] = 0;
                return true;
            } else {
                for (int i = 1; i < roll; ++i) {
                    if (board[i - 1] && (roll - i <= 6)) {
                        board[i - 1] = 0;
                        return true;
                    }
                }
            }
        } else {
            if (!board[roll - 1]) {
                board[roll - 1] = 1;
                return true;
            } else {
                for (int i = 1; i < roll; ++i) {
                    if (!board[i - 1] && (roll - i <= 6)) {
                        board[i - 1] = 1;
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
};