# A Game-Theoretic Analysis of a Stochastic Bit-Flipping Dice Game with Multi-Round Scoring

## 1. Introduction

This paper presents a comprehensive game-theoretic analysis of a novel dice game involving bit manipulation on a 12-bit board, with multiple scoring rounds. The game combines elements of chance (dice rolls) with strategic decision-making, creating a complex environment for player interaction. We analyze the game's structure, strategic considerations, and probabilistic elements to determine optimal play strategies and potential equilibria.

## 2. Game Description

### 2.1 Base Game Rules

1. The game is played on a 12-bit board, initially set to all 1s (0xFFF in hexadecimal).
2. Two players take turns rolling two six-sided dice and summing the result (2-12).
3. On their turn, a player must flip (from 1 to 0) either:
   a) The bit corresponding to the dice sum, or
   b) A combination of bits that sum to the dice roll.
4. A player's turn continues until no valid move is possible.
5. When a player can no longer make a move, their score for the round is the sum of the remaining 1-bits.
6. The board is reset to all 1s, and the other player begins their round.
7. This process is repeated for a total of 5 rounds for each player.
8. The player with the lower total score across all 5 rounds wins.

### 2.2 Extended Game Rules

1. The initial setup is identical to the base game.
2. Player A aims to turn all bits to 0, while Player B aims to return all bits to 1.
3. Players alternate turns, following the same movement rules as the base game.
4. The game ends when either all bits are 0 (Player A wins) or all bits are 1 (Player B wins).


## 3. Game-Theoretic Framework

### 3.1 Game Structure

- **Players**: N = {1, 2}
- **Action Space**: A = {a ∈ ℤ | 1 ≤ a ≤ 12} ∪ {subsets of A that sum to the dice roll}
- **State Space**: S = {0, 1}^12 (4096 possible states) for each turn, S' = ℤ^5 for the overall game score
- **Transition Function**: T: S × A × S → [0, 1], determined by dice roll probabilities and chosen actions
- **Reward Function**: R: S → ℤ, defined as the negative sum of 1-bits at the end of each round

### 3.2 Game Classification

- **Sequential Game**: Players take alternating rounds
- **Stochastic Game**: Outcomes are influenced by random dice rolls
- **Perfect Information**: The complete game state is observable to both players
- **Zero-Sum Game**: One player's gain is exactly balanced by the other's loss

## 4. Probabilistic Analysis

### 4.1 Dice Roll Probabilities

Let P(r) be the probability of rolling a sum r with two six-sided dice:

| Sum (r) | Probability P(r) |
|---------|------------------|
| 2, 12   | 1/36 ≈ 0.0278    |
| 3, 11   | 2/36 ≈ 0.0556    |
| 4, 10   | 3/36 ≈ 0.0833    |
| 5, 9    | 4/36 ≈ 0.1111    |
| 6, 8    | 5/36 ≈ 0.1389    |
| 7       | 6/36 ≈ 0.1667    |

### 4.2 Expected Value of Bit Flips

The expected value (EV) of flipping a bit b is:

EV(b) = b * P(b), for 2 ≤ b ≤ 12

This yields:

| Bit | Expected Value |
|-----|----------------|
| 7   | 1.1667         |
| 8   | 1.1111         |
| 6   | 0.8333         |
| 9   | 1.0000         |
| 5   | 0.5556         |
| 10  | 0.8333         |
| 4   | 0.3333         |
| 11  | 0.6111         |
| 3   | 0.1667         |
| 12  | 0.3333         |
| 2   | 0.0556         |


## 5. Strategic Analysis

### 5.1 Round Strategy

Within each round, players face a stopping problem: they must balance the benefit of continuing to flip bits against the risk of being forced to stop with a high score.

Optimal stopping strategy σ*(s) for state s:
σ*(s) = continue, if E[R(s')|s] < R(s)
        stop, otherwise

Where E[R(s')|s] is the expected reward of the next state given the current state.

### 5.2 Multi-Round Strategy

Players must now consider their strategy across multiple rounds. This introduces several new strategic elements:

1. **Score Management**: Players may adopt different strategies based on their cumulative score relative to their opponent.
2. **Risk Assessment**: The willingness to take risks may vary depending on the round number and score difference.
3. **Opponent Modeling**: Players may adjust their strategy based on the opponent's past performance and perceived strategy.

### 5.3 Nash Equilibrium

Given the increased complexity from multiple rounds, a pure strategy Nash equilibrium is even less likely than in a single-round game. A mixed strategy equilibrium may exist, where players probabilistically choose between different strategic approaches based on the game state and round number.

### 5.4 Subgame Perfect Equilibrium

The game can be modeled as an extensive-form game with chance nodes, where each round is a subgame. A subgame perfect equilibrium could theoretically be computed using backward induction, considering all possible score combinations after each round.

## 6. Markov Decision Process Formulation

The game can be modeled as a series of interconnected Markov Decision Processes (MDPs), one for each round:

- **States**: S × S' (current board state and cumulative scores)
- **Actions**: A (as defined in 3.1)
- **Transition Probabilities**: P(s', score' | s, score, a) determined by dice roll probabilities and chosen actions
- **Rewards**: R(s, score, a, s', score') based on the change in board state and cumulative score

The optimal value function V*(s, score, round) for a player in state s with score score in round round is given by the Bellman equation:

V*(s, score, round) = max_a [R(s, score, a) + γ * Σ_{s', score'} P(s', score' | s, score, a) * V*(s', score', round+1)]

where γ is a discount factor (typically 1 for finite-horizon games).

## 7. Computational Complexity

### 7.1 State Space Complexity

The state space now includes both the board state and the cumulative scores:
|S| * |S'| = 4096 * M^10, where M is the maximum possible score per round (78).

### 7.2 Game Tree Complexity

The game tree complexity is significantly increased due to the multiple rounds. An upper bound on the game tree complexity is O((4096 * M^10)^5), where 5 is the number of rounds.

## 8. Approximate Solutions

Given the increased complexity, approximate solutions become even more crucial:

1. **Monte Carlo Tree Search (MCTS)**: Can be adapted to handle the multi-round structure, with the tree depth corresponding to both in-round decisions and round transitions.
2. **Reinforcement Learning**: Techniques like Q-learning or policy gradients can be particularly effective in learning strategies that balance in-round and across-round considerations.
3. **Heuristic Evaluation Functions**: Can be developed to estimate the value of a given score difference at each round, guiding decision-making.

## 9. Evolutionary Game Theory Perspective

In the context of multiple rounds, evolutionary stability of strategies becomes more complex:

1. **Strategy Space**: Σ = {Round Strategies} × {Multi-Round Strategies}
2. **Fitness Function**: f(σ, Σ) = Expected win rate of strategy σ against the population Σ across all 5 rounds
3. **Evolutionary Stable Strategy (ESS)**: A strategy σ* is ESS if it performs best against both itself and potential mutations across the full 5-round game.

## 10. Conclusion and Future Work

This revised analysis accounts for the multi-round nature of the game, significantly increasing its strategic depth. Key findings include:

1. The game now involves both in-round tactical decisions and across-round strategic planning.
2. Optimal play likely involves adaptive strategies that consider both the current board state and the overall score situation.
3. The increased complexity makes computational approaches even more challenging, necessitating sophisticated approximation methods.

Future work should focus on:

1. Developing and testing AI players that can effectively manage both in-round and across-round strategies.
2. Investigating how human players handle the increased complexity and whether they develop intuitive heuristics for multi-round play.
3. Analyzing the impact of different scoring systems on optimal strategies and game balance.

This revised analysis provides a more accurate theoretical framework for understanding your dice game, accounting for its multi-round scoring system and the strategic implications thereof.