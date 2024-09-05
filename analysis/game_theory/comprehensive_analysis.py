# File: analysis/game_theory/comprehensive_analysis.py

import numpy as np
from scipy.optimize import linprog
import matplotlib.pyplot as plt
import seaborn as sns

class GameTheoryAnalyzer:
    def __init__(self, payoff_matrix):
        self.payoff_matrix = np.array(payoff_matrix)

    def find_nash_equilibrium(self):
        num_strategies = self.payoff_matrix.shape[0]
        c = np.zeros(num_strategies + 1)
        c[-1] = -1
        A_ub = np.column_stack([-self.payoff_matrix.T, np.ones(num_strategies)])
        b_ub = np.zeros(num_strategies)
        A_eq = np.ones((1, num_strategies + 1))
        A_eq[0, -1] = 0
        b_eq = np.ones(1)
        bounds = [(0, None)] * num_strategies + [(None, None)]
        
        res = linprog(c, A_ub=A_ub, b_ub=b_ub, A_eq=A_eq, b_eq=b_eq, bounds=bounds)
        
        if res.success:
            return res.x[:-1] / np.sum(res.x[:-1])
        else:
            return None

    def dominating_strategies(self):
        rows, cols = self.payoff_matrix.shape
        dominated_rows = set()
        dominating_rows = set()

        for i in range(rows):
            for j in range(rows):
                if i != j and not i in dominated_rows:
                    if all(self.payoff_matrix[i] >= self.payoff_matrix[j]):
                        dominated_rows.add(j)
                        dominating_rows.add(i)
        
        return list(dominating_rows)

    def best_response_strategy(self, opponent_strategy):
        expected_payoffs = self.payoff_matrix @ opponent_strategy
        return np.argmax(expected_payoffs)

    def visualize_payoff_matrix(self):
        plt.figure(figsize=(10, 8))
        sns.heatmap(self.payoff_matrix, annot=True, cmap="YlGnBu")
        plt.title("Payoff Matrix")
        plt.xlabel("Player 2 Strategy")
        plt.ylabel("Player 1 Strategy")
        plt.show()

    def evolutionary_stable_strategy(self, iterations=1000, mutation_rate=0.01):
        population = np.random.rand(self.payoff_matrix.shape[0])
        population /= population.sum()

        for _ in range(iterations):
            fitness = self.payoff_matrix @ population
            population = population * fitness
            population /= population.sum()
            
            # Apply mutation
            mutation = np.random.rand(len(population)) * mutation_rate
            mutation /= mutation.sum()
            population = (1 - mutation_rate) * population + mutation

        return population

# Example usage
if __name__ == "__main__":
    # Example payoff matrix
    payoff_matrix = [
        [3, 1, 0],
        [2, 2, 1],
        [0, 3, 3]
    ]

    analyzer = GameTheoryAnalyzer(payoff_matrix)

    print("Nash Equilibrium:", analyzer.find_nash_equilibrium())
    print("Dominating Strategies:", analyzer.dominating_strategies())
    print("Best Response to [0.5, 0.3, 0.2]:", analyzer.best_response_strategy([0.5, 0.3, 0.2]))
    print("Evolutionary Stable Strategy:", analyzer.evolutionary_stable_strategy())

    analyzer.visualize_payoff_matrix()