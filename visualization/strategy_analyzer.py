# File: visualization/strategy_analyzer.py
import matplotlib.pyplot as plt
import seaborn as sns

def visualize_strategy_performance(strategy_results):
    strategies = list(strategy_results.keys())
    win_rates = [result['win_rate'] for result in strategy_results.values()]
    
    plt.figure(figsize=(12, 6))
    sns.barplot(x=strategies, y=win_rates)
    plt.title("Strategy Performance Comparison")
    plt.ylabel("Win Rate")
    plt.xticks(rotation=45)
    plt.show()