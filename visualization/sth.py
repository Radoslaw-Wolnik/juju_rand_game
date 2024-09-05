import json
import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

# Load the JSON data
with open('tournament_results.json', 'r') as f:
    data = json.load(f)

# Prepare data for heatmaps
strategies = ['Random', 'HighestValue', 'HighestProbability', 'BalancedValue', 'Adaptive', 'LookAhead']
game_types = ['base_game', 'extended_game']

for game_type in game_types:
    win_rates = {strategy: [] for strategy in strategies}
    
    for matchup, results in data.items():
        strategy_a, strategy_b = matchup.split('_vs_')
        win_rate = results[game_type]['player_a_win_percentage']
        win_rates[strategy_a].append(win_rate)
    
    df = pd.DataFrame(win_rates, index=strategies)
    
    plt.figure(figsize=(10, 8))
    sns.heatmap(df, annot=True, cmap="YlGnBu", fmt=".1f")
    plt.title(f"{game_type.replace('_', ' ').title()} - Win Rates (%)")
    plt.xlabel("Player B Strategy")
    plt.ylabel("Player A Strategy")
    plt.tight_layout()
    plt.savefig(f"{game_type}_heatmap.png")
    plt.close()

print("Heatmaps have been generated and saved as PNG files.")