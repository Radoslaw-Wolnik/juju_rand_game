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
    data = [[results[s1].get(s2, 0) for s2 in strategies] for