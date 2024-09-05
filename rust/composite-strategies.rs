use rand::Rng;
use std::collections::HashSet;

const ROUNDS: usize = 5;
const MAX_LOOK_AHEAD: usize = 3;

#[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
pub enum StrategyComponent {
    Random,
    HighestValue,
    HighestProbability,
    BalancedValue,
    Adaptive,
    LookAhead,
    ScoreManagement,
    RiskAverse,
    Aggressive,
    PatternRecognition,
}

#[derive(Clone, PartialEq, Eq, Debug)]
pub struct CompositeStrategy {
    components: HashSet<StrategyComponent>,
    name: String,
}

impl CompositeStrategy {
    pub fn new(components: &[StrategyComponent], name: &str) -> Self {
        CompositeStrategy {
            components: components.iter().cloned().collect(),
            name: name.to_string(),
        }
    }

    pub fn contains(&self, component: StrategyComponent) -> bool {
        self.components.contains(&component)
    }

    pub fn name(&self) -> &str {
        &self.name
    }
}

pub struct GameState {
    pub board: u16,
    pub current_round_score: u32,
    pub total_score: u32,
    pub opponent_score: u32,
    pub round: usize,
    pub turn_history: Vec<u16>,
}

impl GameState {
    pub fn new() -> Self {
        GameState {
            board: 0xFFF,
            current_round_score: 0,
            total_score: 0,
            opponent_score: 0,
            round: 0,
            turn_history: Vec::new(),
        }
    }

    pub fn reset_board(&mut self) {
        self.board = 0xFFF;
        self.current_round_score = 0;
        self.turn_history.clear();
    }

    pub fn end_round(&mut self) {
        self.total_score += self.current_round_score;
        self.round += 1;
        self.reset_board();
    }
}

pub fn play_turn(state: &mut GameState, roll: u8, strategy: &CompositeStrategy) -> bool {
    let possible_moves = generate_possible_moves(state.board, roll);
    
    if possible_moves.is_empty() {
        return false;
    }
    
    let chosen_move = choose_move(state, &possible_moves, strategy);
    
    state.board &= !chosen_move;
    state.turn_history.push(chosen_move);
    true
}

fn choose_move(state: &GameState, possible_moves: &[u16], strategy: &CompositeStrategy) -> u16 {
    let mut move_scores: Vec<(u16, f64)> = possible_moves.iter().map(|&m| (m, 0.0)).collect();

    for component in &strategy.components {
        match component {
            StrategyComponent::Random => random_score(&mut move_scores),
            StrategyComponent::HighestValue => highest_value_score(&mut move_scores),
            StrategyComponent::HighestProbability => highest_probability_score(&mut move_scores),
            StrategyComponent::BalancedValue => balanced_value_score(&mut move_scores),
            StrategyComponent::Adaptive => adaptive_score(state, &mut move_scores),
            StrategyComponent::LookAhead => look_ahead_score(state, &mut move_scores),
            StrategyComponent::ScoreManagement => score_management_score(state, &mut move_scores),
            StrategyComponent::RiskAverse => risk_averse_score(state, &mut move_scores),
            StrategyComponent::Aggressive => aggressive_score(state, &mut move_scores),
            StrategyComponent::PatternRecognition => pattern_recognition_score(state, &mut move_scores),
        }
    }

    move_scores.iter().max_by(|a, b| a.1.partial_cmp(&b.1).unwrap()).unwrap().0
}

fn generate_possible_moves(board: u16, roll: u8) -> Vec<u16> {
    let mut moves = Vec::new();
    if board & (1 << (roll - 1)) != 0 {
        moves.push(1 << (roll - 1));
    }
    
    for i in 1..(1 << 12) {
        if i.count_ones() as u8 <= roll && (board & i) == i {
            let sum: u8 = (0..12).filter(|&j| i & (1 << j) != 0).map(|j| j as u8 + 1).sum();
            if sum == roll {
                moves.push(i);
            }
        }
    }
    
    moves
}

fn random_score(move_scores: &mut [(u16, f64)]) {
    let mut rng = rand::thread_rng();
    for (_, score) in move_scores.iter_mut() {
        *score += rng.gen::<f64>();
    }
}

fn highest_value_score(move_scores: &mut [(u16, f64)]) {
    for (m, score) in move_scores.iter_mut() {
        *score += m.trailing_zeros() as f64;
    }
}

fn highest_probability_score(move_scores: &mut [(u16, f64)]) {
    for (m, score) in move_scores.iter_mut() {
        let center = (m.trailing_zeros() as i32 - 7).abs();
        *score += 7.0 - center as f64;
    }
}

fn balanced_value_score(move_scores: &mut [(u16, f64)]) {
    for (m, score) in move_scores.iter_mut() {
        let value = m.trailing_zeros() as f64;
        let probability = 7.0 - (value - 7.0).abs();
        *score += value * probability;
    }
}

fn adaptive_score(state: &GameState, move_scores: &mut [(u16, f64)]) {
    let score_diff = state.total_score as i32 - state.opponent_score as i32;
    for (m, score) in move_scores.iter_mut() {
        if score_diff > 0 {
            // If ahead, prefer safer moves
            *score += highest_probability_score(&mut [(m, 0.0)])[0].1;
        } else {
            // If behind, prefer higher value moves
            *score += highest_value_score(&mut [(m, 0.0)])[0].1;
        }
    }
}

fn look_ahead_score(state: &GameState, move_scores: &mut [(u16, f64)]) {
    for (m, score) in move_scores.iter_mut() {
        *score += evaluate_move(state, *m, MAX_LOOK_AHEAD) as f64;
    }
}

fn score_management_score(state: &GameState, move_scores: &mut [(u16, f64)]) {
    let score_diff = state.total_score as i32 - state.opponent_score as i32;
    let rounds_left = ROUNDS - state.round;
    for (m, score) in move_scores.iter_mut() {
        if score_diff > 10 * rounds_left as i32 {
            // If far ahead, play conservatively
            *score += highest_probability_score(&mut [(m, 0.0)])[0].1;
        } else if score_diff < -10 * rounds_left as i32 {
            // If far behind, play aggressively
            *score += highest_value_score(&mut [(m, 0.0)])[0].1;
        } else {
            // If close, play balanced
            *score += balanced_value_score(&mut [(m, 0.0)])[0].1;
        }
    }
}

fn risk_averse_score(state: &GameState, move_scores: &mut [(u16, f64)]) {
    for (m, score) in move_scores.iter_mut() {
        let risk = m.count_ones() as f64;  // More bits flipped = higher risk
        *score -= risk;
    }
}

fn aggressive_score(state: &GameState, move_scores: &mut [(u16, f64)]) {
    for (m, score) in move_scores.iter_mut() {
        let aggression = m.count_ones() as f64;  // More bits flipped = more aggressive
        *score += aggression;
    }
}

fn pattern_recognition_score(state: &GameState, move_scores: &mut [(u16, f64)]) {
    // This is a simplified pattern recognition. In a real implementation, 
    // you might want to use more sophisticated pattern matching algorithms.
    for (m, score) in move_scores.iter_mut() {
        if state.turn_history.contains(m) {
            *score += 5.0;  // Favor moves that have been successful in the past
        }
    }
}

fn evaluate_move(state: &GameState, move_: u16, depth: usize) -> i32 {
    if depth == 0 {
        return -(state.board.count_ones() as i32);
    }
    
    let mut new_state = state.clone();
    new_state.board &= !move_;
    
    let mut best_score = i32::MIN;
    for roll in 2..=12 {
        let possible_moves = generate_possible_moves(new_state.board, roll);
        if possible_moves.is_empty() {
            return -(new_state.board.count_ones() as i32);
        }
        for &m in &possible_moves {
            let score = evaluate_move(&new_state, m, depth - 1);
            best_score = best_score.max(score);
        }
    }
    
    best_score
}

pub fn simulate_game(strategy_a: &CompositeStrategy, strategy_b: &CompositeStrategy) -> (u32, u32) {
    let mut state_a = GameState::new();
    let mut state_b = GameState::new();
    let mut rng = rand::thread_rng();

    for _ in 0..ROUNDS {
        play_round(&mut state_a, &mut state_b, strategy_a, &mut rng);
        play_round(&mut state_b, &mut state_a, strategy_b, &mut rng);
    }

    (state_a.total_score, state_b.total_score)
}

fn play_round(state: &mut GameState, opponent_state: &mut GameState, strategy: &CompositeStrategy, rng: &mut impl Rng) {
    state.reset_board();
    while play_turn(state, roll_dice(rng), strategy) {}
    state.current_round_score = state.board.count_ones() as u32;
    state.end_round();
    state.opponent_score = opponent_state.total_score;
}

fn roll_dice(rng: &mut impl Rng) -> u8 {
    rng.gen_range(1..=6) + rng.gen_range(1..=6)
}

pub fn run_tournament(strategies: &[CompositeStrategy], num_simulations: usize) -> Vec<(String, usize, u32)> {
    let mut results = vec![(String::new(), 0, 0); strategies.len()];

    for (i, strategy_a) in strategies.iter().enumerate() {
        for (j, strategy_b) in strategies.iter().enumerate() {
            if i < j {
                let mut a_wins = 0;
                let mut b_wins = 0;

                for _ in 0..num_simulations {
                    let (score_a, score_b) = simulate_game(strategy_a, strategy_b);
                    if score_a < score_b {
                        a_wins += 1;
                    } else if score_b < score_a {
                        b_wins += 1;
                    }
                }

                results[i].1 += a_wins;
                results[i].2 += num_simulations as u32;
                results[j].1 += b_wins;
                results[j].2 += num_simulations as u32;
            }
        }
        results[i].0 = strategy_a.name().to_string();
    }

    results
}

fn main() {
    let strategies = vec![
        CompositeStrategy::new(&[StrategyComponent::Random], "Random"),
        CompositeStrategy::new(&[StrategyComponent::HighestValue], "Highest Value"),
        CompositeStrategy::new(&[StrategyComponent::HighestProbability], "Highest Probability"),
        CompositeStrategy::new(&[StrategyComponent::BalancedValue], "Balanced Value"),
        CompositeStrategy::new(&[StrategyComponent::Adaptive, StrategyComponent::ScoreManagement], "Adaptive Score Management"),
        CompositeStrategy::new(&[StrategyComponent::LookAhead, StrategyComponent::ScoreManagement], "Look Ahead Score Management"),
        CompositeStrategy::new(&[StrategyComponent::LookAhead, StrategyComponent::Adaptive, StrategyComponent::ScoreManagement], "Advanced Adaptive"),
        CompositeStrategy::new(&[StrategyComponent::BalancedValue, StrategyComponent::RiskAverse, StrategyComponent::PatternRecognition], "Cautious Pattern Learner"),
        CompositeStrategy::new(&[StrategyComponent::HighestValue, StrategyComponent::Aggressive, StrategyComponent::LookAhead], "Aggressive Planner"),
    ];

    let results = run_tournament(&strategies, 10000);

    for (name, wins, total_games) in results {
        println!("{}: Win rate = {:.2}%", name, (wins as f64 / total_games as f64) * 100.0);
    }
}