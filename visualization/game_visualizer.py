# File: visualization/game_visualizer.py

import matplotlib.pyplot as plt
import seaborn as sns

def visualize_game_state(game_state):
    plt.figure(figsize=(10, 2))
    sns.heatmap([[(game_state.board >> i) & 1 for i in range(12)]], 
                cmap="YlGnBu", cbar=False, xticklabels=range(1, 13))
    plt.title(f"Game State (Round {game_state.round})")
    plt.show()