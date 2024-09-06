#pragma once
#include <torch/torch.h>
#include <vector>
#include <random>
#include "../include/game.hpp"

class DQNAgent {
private:
    struct DQN : torch::nn::Module {
        DQN(int input_dim, int output_dim);
        torch::Tensor forward(torch::Tensor x);
        torch::nn::Linear fc1, fc2, fc3;
    };

    DQN model;
    torch::optim::Adam optimizer;
    std::mt19937 rng;
    float epsilon;
    float gamma;

public:
    DQNAgent(int state_dim, int action_dim, float learning_rate = 0.001, float epsilon = 0.1, float gamma = 0.99);
    uint16_t chooseAction(const GameState& state, uint8_t roll);
    void train(const GameState& state, uint16_t action, float reward, const GameState& next_state, bool done);
    void save(const std::string& filename);
    void load(const std::string& filename);
};