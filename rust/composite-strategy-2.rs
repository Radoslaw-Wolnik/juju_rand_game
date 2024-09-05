use std::collections::HashSet;

#[derive(Clone, Copy, PartialEq, Eq, Debug, Hash)]
pub enum StrategyComponent {
    Random,
    HighestValue,
    HighestProbability,
    BalancedValue,
    Adaptive,
    LookAhead,
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
    pub score: u32,
}

impl GameState {
    pub fn new() -> Self {
        GameState { board: 0xFFF, score: 0 }
    }
}

pub fn play_turn(state: &mut GameState, roll: u8, strategy: &CompositeStrategy, is_player_a: bool) -> bool {
    let mut possible_moves = Vec::new();
    
    // Generate all possible moves
    for bit in 1..=12 {
        if (state.board & (1 << (bit - 1)) != 0) == is_player_a && (bit <= roll || is_subset_sum(roll as i32 - bit as i32, state.board, is_player_a)) {
            possible_moves.push(bit);
        }
    }
    
    if possible_moves.is_empty() {
        return false;
    }
    
    let chosen_bit = if strategy.contains(StrategyComponent::LookAhead) {
        look_ahead_choice(state, &possible_moves, is_player_a)
    } else if strategy.contains(StrategyComponent::Adaptive) {
        adaptive_choice(state, &possible_moves, is_player_a)
    } else if strategy.contains(StrategyComponent::BalancedValue) {
        balanced_value_choice(&possible_moves)
    } else if strategy.contains(StrategyComponent::HighestValue) {
        *possible_moves.iter().max().unwrap()
    } else if strategy.contains(StrategyComponent::HighestProbability) {
        highest_probability_choice(&possible_moves)
    } else {
        // Random strategy or fallback
        possible_moves[rand::thread_rng().gen_range(0..possible_moves.len())]
    };
    
    if is_player_a {
        state.board &= !(1 << (chosen_bit - 1));
    } else {
        state.board |= 1 << (chosen_bit - 1);
    }
    true
}

// Implement the individual strategy component functions here
// (look_ahead_choice, adaptive_choice, balanced_value_choice, highest_probability_choice, etc.)

fn is_subset_sum(target: i32, board: u16, is_player_a: bool) -> bool {
    // Implementation remains the same
}

// Add other necessary helper functions