from dqn_agent import DQNAgent
from cpp.base_game import BaseGame  # You'll need to create Python bindings for your C++ code
import numpy as np
import torch

def train_dqn(episodes=10000):
    agent = DQNAgent(state_dim=12, action_dim=12)
    game = BaseGame()

    for episode in range(episodes):
        state = game.reset()
        done = False
        total_reward = 0

        while not done:
            action = agent.choose_action(state, epsilon=0.1)
            next_state, reward, done = game.step(action)
            agent.train(state, action, reward, next_state, done)
            state = next_state
            total_reward += reward

        if episode % 100 == 0:
            print(f"Episode {episode}, Total Reward: {total_reward}")

    # Save the trained model
    torch.save(agent.dqn.state_dict(), "trained_dqn.pth")

if __name__ == "__main__":
    train_dqn()