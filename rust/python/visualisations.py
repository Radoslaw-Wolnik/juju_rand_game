# File: python/visualizations.py

import matplotlib.pyplot as plt
import seaborn as sns
import io
import base64

def create_strategy_comparison(results):
    strategies = list(results.keys())
    win_rates = [sum(result.values()) / len(result) for result in results.values()]
    
    plt.figure(figsize=(12, 6))
    sns.barplot(x=strategies, y=win_rates)
    plt.title("Strategy Performance Comparison")
    plt.ylabel("Average Win Rate")
    plt.xticks(rotation=45)
    
    img = io.BytesIO()
    plt.savefig(img, format='png', bbox_inches='tight')
    img.seek(0)
    return base64.b64encode(img.getvalue()).decode()

def create_tournament_heatmap(results):
    strategies = list(results.keys())
    data = [[results[s1].get(s2, 0) for s2 in strategies] for s1 in strategies]
    
    plt.figure(figsize=(10, 8))
    sns.heatmap(data, annot=True, cmap="YlGnBu", xticklabels=strategies, yticklabels=strategies)
    plt.title("Tournament Results")
    plt.xlabel("Opponent Strategy")
    plt.ylabel("Strategy")
    
    img = io.BytesIO()
    plt.savefig(img, format='png', bbox_inches='tight')
    img.seek(0)
    return base64.b64encode(img.getvalue()).decode()

def create_game_state_visualization(game_state):
    plt.figure(figsize=(10, 2))
    sns.heatmap([[(game_state['board'] >> i) & 1 for i in range(12)]], 
                cmap="YlGnBu", cbar=False, xticklabels=range(1, 13))
    plt.title(f"Game State (Round {game_state['round']})")
    
    img = io.BytesIO()
    plt.savefig(img, format='png', bbox_inches='tight')
    img.seek(0)
    return base64.b64encode(img.getvalue()).decode()