# File: python/tournament.py

import json
from typing import List, Dict
from game_logic import Game, Strategy

def run_tournament(strategies: List[Strategy], num_games: int) -> Dict[str, Dict[str, float]]:
    results = {}

    for i, strategy1 in enumerate(strategies):
        for strategy2 in strategies[i+1:]:
            wins1 = wins2 = 0
            for _ in range(num_games):
                game = Game()
                winner = game.play(strategy1, strategy2)
                if winner == 0:
                    wins1 += 1
                elif winner == 1:
                    wins2 += 1

            win_rate1 = wins1 / num_games
            win_rate2 = wins2 / num_games

            results.setdefault(strategy1.name, {})[strategy2.name] = win_rate1
            results.setdefault(strategy2.name, {})[strategy1.name] = win_rate2

    return results

def save_tournament_results(results: Dict[str, Dict[str, float]], filename: str):
    with open(filename, 'w') as f:
        json.dump(results, f, indent=2)

def load_tournament_results(filename: str) -> Dict[str, Dict[str, float]]:
    with open(filename, 'r') as f:
        return json.load(f)
