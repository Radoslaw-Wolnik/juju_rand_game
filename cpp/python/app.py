from flask import Flask, render_template, request, jsonify
from game_logic import Game, Strategy
from tournament import run_tournament
from visualizations import create_strategy_comparison, create_tournament_heatmap, create_game_state_visualization
import json

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/play', methods=['POST'])
def play():
    data = request.json
    game = Game()
    human_strategy = Strategy.from_name(data['human_strategy'])
    ai_strategy = Strategy.from_name(data['ai_strategy'])
    result = game.play(human_strategy, ai_strategy)
    return jsonify(result)

@app.route('/simulate', methods=['POST'])
def simulate():
    data = request.json
    strategies = [Strategy.from_name(s) for s in data['strategies']]
    num_games = data['num_games']
    results = run_tournament(strategies, num_games)
    
    comparison_chart = create_strategy_comparison(results)
    heatmap_chart = create_tournament_heatmap(results)
    
    return jsonify({
        'results': results,
        'comparison_chart': comparison_chart,
        'heatmap_chart': heatmap_chart
    })

@app.route('/visualize_game_state', methods=['POST'])
def visualize_game_state():
    data = request.json
    game_state = data['game_state']
    visualization = create_game_state_visualization(game_state)
    return jsonify({'visualization': visualization})

if __name__ == '__main__':
    app.run(debug=True)