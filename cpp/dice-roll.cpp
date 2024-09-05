// Add this function to all C++ game classes
private:
    int rollTwoDice() {
        return dist(gen) + dist(gen);
    }

// Replace the random number generation in all game classes with:
std::uniform_int_distribution<> dist{1, 6};
int roll = rollTwoDice();