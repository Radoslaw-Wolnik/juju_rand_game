# File: web/app.py

from flask import Flask, render_template, request, jsonify
from game_logic import Game, Strategy  # Import your game logic

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
    # Implement analysis logic here
    return jsonify({'analysis': 'Analysis result'})

if __name__ == '__main__':
    app.run(debug=True)