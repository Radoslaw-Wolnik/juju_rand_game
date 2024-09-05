# File: visualization/tournament_visualizer.py
import matplotlib.pyplot as plt
import seaborn as sns

def visualize_tournament_results(tournament_results):
    strategies = list(tournament_results.keys())
    data = [[tournament_results[s1][s2] for s2 in strategies] for s1 in strategies]
    
    plt.figure(figsize=(10, 8))
    sns.heatmap(data, annot=True, cmap="YlGnBu", xticklabels=strategies, yticklabels=strategies)
    plt.title("Tournament Results")
    plt.xlabel("Opponent Strategy")
    plt.ylabel("Strategy")
    plt.show()