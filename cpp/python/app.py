# File: app.py

from flask import Flask, render_template, request, jsonify
import json
from game_logic import Game, Strategy  # Import your game logic
from visualizations import create_strategy_comparison, create_tournament_heatmap, create_game_state_visualization

app = Flask(__name__)

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/play', methods=['POST'])
def play():
    data = request.json
    game = Game()
    strategy = Strategy.from_name(data['strategy'])
    game.set_player_strategy(strategy)
    result = game.play()
    return jsonify(result)

@app.route('/analyze', methods=['POST'])
def analyze():
    data = request.json
    with open('tournament_results.json', 'r') as f:
        results = json.load(f)
    
    strategy_comparison = create_strategy_comparison(results)
    tournament_heatmap = create_tournament_heatmap(results)
    
    return jsonify({
        'strategy_comparison': strategy_comparison,
        'tournament_heatmap': tournament_heatmap
    })

@app.route('/visualize_game', methods=['POST'])
def visualize_game():
    data = request.json
    game_state = data['game_state']
    visualization = create_game_state_visualization(game_state)
    return jsonify({'visualization': visualization})

if __name__ == '__main__':
    app.run(debug=True)