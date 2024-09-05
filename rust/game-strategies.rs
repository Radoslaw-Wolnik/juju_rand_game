use rand::Rng;

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum Strategy {
    Random,
    HighestValue,
    HighestProbability,
    BalancedValue,
    Adaptive,
    LookAhead,
}

pub struct GameState {
    pub board: u16,
    pub score: u32,
}

impl GameState {
    pub fn new() -> Self {
        GameState { board: 0xFFF, score: 0 }
    }
}

pub fn play_turn(state: &mut GameState, roll: u8, strategy: Strategy, is_player_a: bool) -> bool {
    match strategy {
        Strategy::Random => random_strategy(state, roll, is_player_a),
        Strategy::HighestValue => highest_value_strategy(state, roll, is_player_a),
        Strategy::HighestProbability => highest_probability_strategy(state, roll, is_player_a),
        Strategy::BalancedValue => balanced_value_strategy(state, roll, is_player_a),
        Strategy::Adaptive => adaptive_strategy(state, roll, is_player_a),
        Strategy::LookAhead => look_ahead_strategy(state, roll, is_player_a),
    }
}

fn random_strategy(state: &mut GameState, roll: u8, is_player_a: bool) -> bool {
    let mut rng = rand::thread_rng();
    let available_bits: Vec<u8> = (1..=12)
        .filter(|&i| (state.board & (1 << (i - 1)) != 0) == is_player_a)
        .collect();
    
    if available_bits.is_empty() {
        return false;
    }
    
    let chosen_bit = available_bits[rng.gen_range(0..available_bits.len())];
    if is_player_a {
        state.board &= !(1 << (chosen_bit - 1));
    } else {
        state.board |= 1 << (chosen_bit - 1);
    }
    true
}

fn highest_value_strategy(state: &mut GameState, roll: u8, is_player_a: bool) -> bool {
    for bit in (1..=12).rev() {
        if (state.board & (1 << (bit - 1)) != 0) == is_player_a && (bit <= roll || is_subset_sum(roll as i32 - bit as i32, state.board, is_player_a)) {
            if is_player_a {
                state.board &= !(1 << (bit - 1));
            } else {
                state.board |= 1 << (bit - 1);
            }
            return true;
        }
    }
    false
}

fn highest_probability_strategy(state: &mut GameState, roll: u8, is_player_a: bool) -> bool {
    let probabilities = [7, 6, 8, 5, 9, 4, 10, 3, 11, 2, 12];
    for &bit in &probabilities {
        if (state.board & (1 << (bit - 1)) != 0) == is_player_a && (bit <= roll || is_subset_sum(roll as i32 - bit as i32, state.board, is_player_a)) {
            if is_player_a {
                state.board &= !(1 << (bit - 1));
            } else {
                state.board |= 1 << (bit - 1);
            }
            return true;
        }
    }
    false
}

fn balanced_value_strategy(state: &mut GameState, roll: u8, is_player_a: bool) -> bool {
    let balanced_order = [7, 8, 6, 9, 5, 10, 4, 11, 3, 12, 2];
    for &bit in &balanced_order {
        if (state.board & (1 << (bit - 1)) != 0) == is_player_a && (bit <= roll || is_subset_sum(roll as i32 - bit as i32, state.board, is_player_a)) {
            if is_player_a {
                state.board &= !(1 << (bit - 1));
            } else {
                state.board |= 1 << (bit - 1);
            }
            return true;
        }
    }
    false
}

fn adaptive_strategy(state: &mut GameState, roll: u8, is_player_a: bool) -> bool {
    let bits_on = state.board.count_ones();
    if bits_on > 6 {
        highest_value_strategy(state, roll, is_player_a)
    } else {
        highest_probability_strategy(state, roll, is_player_a)
    }
}

fn look_ahead_strategy(state: &mut GameState, roll: u8, is_player_a: bool) -> bool {
    let mut best_move = None;
    let mut best_score = if is_player_a { std::i32::MAX } else { std::i32::MIN };

    for bit in 1..=12 {
        if (state.board & (1 << (bit - 1)) != 0) == is_player_a && (bit <= roll || is_subset_sum(roll as i32 - bit as i32, state.board, is_player_a)) {
            let mut new_state = *state;
            if is_player_a {
                new_state.board &= !(1 << (bit - 1));
            } else {
                new_state.board |= 1 << (bit - 1);
            }
            
            let score = evaluate_position(&new_state, is_player_a);
            if (is_player_a && score < best_score) || (!is_player_a && score > best_score) {
                best_score = score;
                best_move = Some(bit);
            }
        }
    }

    if let Some(bit) = best_move {
        if is_player_a {
            state.board &= !(1 << (bit - 1));
        } else {
            state.board |= 1 << (bit - 1);
        }
        true
    } else {
        false
    }
}

fn is_subset_sum(target: i32, board: u16, is_player_a: bool) -> bool {
    if target == 0 {
        return true;
    }
    if target < 0 {
        return false;
    }
    for bit in 1..=12 {
        if (board & (1 << (bit - 1)) != 0) == is_player_a && is_subset_sum(target - bit as i32, board & !(1 << (bit - 1)), is_player_a) {
            return true;
        }
    }
    false
}

fn evaluate_position(state: &GameState, is_player_a: bool) -> i32 {
    let bits_on = state.board.count_ones();
    if is_player_a {
        -(bits_on as i32)
    } else {
        bits_on as i32
    }
}