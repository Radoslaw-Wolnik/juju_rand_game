# Game Theory Analysis

This module provides comprehensive game theory analysis tools for the dice game.

## Features

- Nash Equilibrium calculation
- Identification of dominating strategies
- Best response strategy calculation
- Visualization of payoff matrices
- Evolutionary stable strategy simulation

## Requirements

- Python 3.7+
- NumPy
- SciPy
- Matplotlib
- Seaborn

Install dependencies with:

```bash
pip install numpy scipy matplotlib seaborn
```

## Usage

```python
from comprehensive_analysis import GameTheoryAnalyzer

# Create a payoff matrix
payoff_matrix = [
    [3, 1, 0],
    [2, 2, 1],
    [0, 3, 3]
]

analyzer = GameTheoryAnalyzer(payoff_matrix)

# Find Nash Equilibrium
ne = analyzer.find_nash_equilibrium()
print("Nash Equilibrium:", ne)

# Identify dominating strategies
dom_strategies = analyzer.dominating_strategies()
print("Dominating Strategies:", dom_strategies)

# Visualize payoff matrix
analyzer.visualize_payoff_matrix()

# Find evolutionary stable strategy
ess = analyzer.evolutionary_stable_strategy()
print("Evolutionary Stable Strategy:", ess)
```

## Extending the Analysis

To apply this analysis to your dice game:

1. Generate payoff matrices by simulating games between different strategies.
2. Use these payoff matrices with the `GameTheoryAnalyzer` to gain insights into optimal play.
3. Extend the analyzer to handle the specific characteristics of your game (e.g., imperfect information, sequential moves).

Future improvements:
- Implement subgame perfect equilibrium for multi-round games.
- Add Bayesian game analysis for scenarios with incomplete information.
- Implement Monte Carlo Tree Search for more complex game states.
- Create a dynamic payoff matrix that updates based on the current game state.