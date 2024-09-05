use rand::Rng;
use std::collections::HashSet;

const ROUNDS: usize = 5;
const MAX_LOOK_AHEAD: usize = 3; // Maximum number of moves to look ahead

#[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
pub enum StrategyComponent {
    Random,
    HighestValue,
    HighestProbability,
    BalancedValue,
    Adaptive,
    LookAhead,
    ScoreManagement,
}

#[derive(Clone, PartialEq, Eq, Debug)]
pub struct CompositeStrategy {
    components: HashSet<StrategyComponent>,
}

impl CompositeStrategy {
    pub fn new(components: &[StrategyComponent]) -> Self {
        CompositeStrategy {
            components: components.iter().cloned().collect(),
        }
    }

    pub fn contains(&self, component: StrategyComponent) -> bool {
        self.components.contains(&component)
    }
}

pub struct GameState {
    pub board: u16,
    pub current_round_score: u32,
    pub total_score: u32,
    pub opponent_score: u32,
    pub round: usize,
}

impl GameState {
    pub fn new() -> Self {
        GameState {
            board: 0xFFF,
            current_round_score: 0,
            total_score: 0,
            opponent_score: 0,
            round: 0,
        }
    }

    pub fn reset_board(&mut self) {
        self.board = 0xFFF;
        self.current_round_score = 0;
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
    
    let chosen_move = if strategy.contains(StrategyComponent::LookAhead) {
        look_ahead_choice(state, &possible_moves)
    } else if strategy.contains(StrategyComponent::Adaptive) {
        adaptive_choice(state, &possible_moves)
    } else if strategy.contains(StrategyComponent::ScoreManagement) {
        score_management_choice(state, &possible_moves)
    } else if strategy.contains(StrategyComponent::BalancedValue) {
        balanced_value_choice(&possible_moves)
    } else if strategy.contains(StrategyComponent::HighestValue) {
        highest_value_choice(&possible_moves)
    } else if strategy.contains(StrategyComponent::HighestProbability) {
        highest_probability_choice(&possible_moves)
    } else {
        random_choice(&possible_moves)
    };
    
    state.board &= !chosen_move;
    true
}

fn generate_possible_moves(board: u16, roll: u8) -> Vec<u16> {
    let mut moves = Vec::new();
    if board & (1 << (roll - 1)) != 0 {
        moves.push(1 << (roll - 1));
    }
    
    // Generate all possible subsets
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

fn look_ahead_choice(state: &GameState, possible_moves: &[u16]) -> u16 {
    possible_moves.iter()
        .max_by_key(|&&m| evaluate_move(state, m, MAX_LOOK_AHEAD))
        .cloned()
        .unwrap()
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

fn adaptive_choice(state: &GameState, possible_moves: &[u16]) -> u16 {
    if state.round < 2 || state.total_score > state.opponent_score {
        highest_value_choice(possible_moves)
    } else {
        balanced_value_choice(possible_moves)
    }
}

fn score_management_choice(state: &GameState, possible_moves: &[u16]) -> u16 {
    let score_difference = state.total_score as i32 - state.opponent_score as i32;
    let remaining_rounds = ROUNDS - state.round;
    
    if score_difference > 10 * remaining_rounds as i32 {
        // If significantly ahead, play conservatively
        highest_probability_choice(possible_moves)
    } else if score_difference < -10 * remaining_rounds as i32 {
        // If significantly behind, play aggressively
        highest_value_choice(possible_moves)
    } else {
        // Otherwise, play balanced
        balanced_value_choice(possible_moves)
    }
}

fn balanced_value_choice(possible_moves: &[u16]) -> u16 {
    possible_moves.iter()
        .max_by_key(|&&m| {
            let value = m.trailing_zeros();
            let probability = 6 - (value as i32 - 7).abs();
            value as i32 * probability
        })
        .cloned()
        .unwrap()
}

fn highest_value_choice(possible_moves: &[u16]) -> u16 {
    *possible_moves.iter().max().unwrap()
}

fn highest_probability_choice(possible_moves: &[u16]) -> u16 {
    *possible_moves.iter().min_by_key(|&&m| (m.trailing_zeros() as i32 - 7).abs()).unwrap()
}

fn random_choice(possible_moves: &[u16]) -> u16 {
    let mut rng = rand::thread_rng();
    possible_moves[rng.gen_range(0..possible_moves.len())]
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

pub fn run_tournament(strategies: &[CompositeStrategy], num_simulations: usize) -> Vec<(usize, usize, u32)> {
    let mut results = vec![(0, 0, 0); strategies.len()];

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
    }

    results.iter_mut().enumerate().for_each(|(i, r)| r.0 = i);
    results
}

fn main() {
    let strategies = vec![
        CompositeStrategy::new(&[StrategyComponent::Random]),
        CompositeStrategy::new(&[StrategyComponent::HighestValue]),
        CompositeStrategy::new(&[StrategyComponent::HighestProbability]),
        CompositeStrategy::new(&[StrategyComponent::BalancedValue]),
        CompositeStrategy::new(&[StrategyComponent::Adaptive, StrategyComponent::ScoreManagement]),
        CompositeStrategy::new(&[StrategyComponent::LookAhead, StrategyComponent::ScoreManagement]),
    ];

    let results = run_tournament(&strategies, 10000);

    for (i, wins, total_games) in results {
        println!("Strategy {}: Win rate = {:.2}%", i, (wins as f64 / total_games as f64) * 100.0);
    }
}