#include "dqn_agent.hpp"
#include <algorithm>

DQNAgent::DQN::DQN(int input_dim, int output_dim)
    : fc1(input_dim, 64), fc2(64, 64), fc3(64, output_dim) {
    register_module("fc1", fc1);
    register_module("fc2", fc2);
    register_module("fc3", fc3);
}

torch::Tensor DQNAgent::DQN::forward(torch::Tensor x) {
    x = torch::relu(fc1->forward(x));
    x = torch::relu(fc2->forward(x));
    return fc3->forward(x);
}

DQNAgent::DQNAgent(int state_dim, int action_dim, float learning_rate, float epsilon, float gamma)
    : model(state_dim, action_dim),
      optimizer(model.parameters(), torch::optim::AdamOptions(learning_rate)),
      rng(std::random_device{}()),
      epsilon(epsilon),
      gamma(gamma) {}

uint16_t DQNAgent::chooseAction(const GameState& state, uint8_t roll) {
    if (std::uniform_real_distribution<>(0, 1)(rng) < epsilon) {
        // Explore: choose a random action
        std::vector<uint16_t> valid_moves;
        for (int i = 1; i <= roll; ++i) {
            if (state.board & (1 << (i - 1))) {
                valid_moves.push_back(1 << (i - 1));
            }
        }
        if (valid_moves.empty()) return 0;
        return valid_moves[std::uniform_int_distribution<>(0, valid_moves.size() - 1)(rng)];
    } else {
        // Exploit: choose the best action according to the model
        torch::NoGradGuard no_grad;
        torch::Tensor state_tensor = torch::from_blob(const_cast<uint16_t*>(&state.board), {1, 1}, torch::kInt16).to(torch::kFloat32);
        torch::Tensor q_values = model->forward(state_tensor);
        int64_t best_action = q_values.argmax().item<int64_t>();
        return 1 << best_action;
    }
}

void DQNAgent::train(const GameState& state, uint16_t action, float reward, const GameState& next_state, bool done) {
    torch::Tensor state_tensor = torch::from_blob(const_cast<uint16_t*>(&state.board), {1, 1}, torch::kInt16).to(torch::kFloat32);
    torch::Tensor next_state_tensor = torch::from_blob(const_cast<uint16_t*>(&next_state.board), {1, 1}, torch::kInt16).to(torch::kFloat32);
    torch::Tensor action_tensor = torch::tensor({static_cast<int64_t>(__builtin_ctz(action))});
    torch::Tensor reward_tensor = torch::tensor({reward});

    torch::Tensor q_values = model->forward(state_tensor);
    torch::Tensor next_q_values = model->forward(next_state_tensor);
    torch::Tensor q_value = q_values.gather(1, action_tensor.unsqueeze(1)).squeeze(1);
    torch::Tensor next_q_value = next_q_values.max(1).values;
    torch::Tensor expected_q_value = reward_tensor + gamma * next_q_value * (1 - done);

    torch::Tensor loss = torch::mse_loss(q_value, expected_q_value.detach());

    optimizer.zero_grad();
    loss.backward();
    optimizer.step();
}

void DQNAgent::save(const std::string& filename) {
    torch::save(model, filename);
}

void DQNAgent::load(const std::string& filename) {
    torch::load(model, filename);
}