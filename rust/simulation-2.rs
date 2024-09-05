use rand::Rng;
use serde_json::json;
use std::fs::File;
use std::io::Write;
use std::time::Instant;
use rayon::prelude::*;

mod strategies;
use strategies::{StrategyComponent, CompositeStrategy, GameState, play_turn};

const NUM_SIMULATIONS: usize = 100_000;
const NUM_ROUNDS_BASE_GAME: usize = 5;

fn roll_dice() -> u8 {
    let mut rng = rand::thread_rng();
    rng.gen_range(1..=6) + rng.gen_range(1..=6)
}

fn simulate_base_game(strategy_a: &CompositeStrategy, strategy_b: &CompositeStrategy) -> (u32, u32) {
    let mut state_a = GameState::new();
    let mut state_b = GameState::new();
    
    for _ in 0..NUM_ROUNDS_BASE_GAME {
        while play_turn(&mut state_a, roll_dice(), strategy_a, true) {}
        state_a.score += state_a.board.count_ones();
        
        while play_turn(&mut state_b, roll_dice(), strategy_b, true) {}
        state_b.score += state_b.board.count_ones();
    }
    
    (state_a.score, state_b.score)
}

fn simulate_extended_game(strategy_a: &CompositeStrategy, strategy_b: &CompositeStrategy) -> bool {
    let mut state = GameState::new();
    let mut is_player_a_turn = true;
    
    loop {
        let roll = roll_dice();
        if !play_turn(&mut state, roll, if is_player_a_turn { strategy_a } else { strategy_b }, is_player_a_turn) {
            is_player_a_turn = !is_player_a_turn;
        }
        
        if state.board == 0 {
            return true;  // Player A wins
        } else if state.board == 0xFFF {
            return false;  // Player B wins
        }
    }
}

fn run_tournament(strategies: &[CompositeStrategy]) -> serde_json::Value {
    let mut results = json!({});

    for (i, strategy_a) in strategies.iter().enumerate() {
        for (j, strategy_b) in strategies.iter().enumerate() {
            let base_game_results: Vec<(u32, u32)> = (0..NUM_SIMULATIONS)
                .into_par_iter()
                .map(|_| simulate_base_game(strategy_a, strategy_b))
                .collect();
            
            let extended_game_results: Vec<bool> = (0..NUM_SIMULATIONS)
                .into_par_iter()
                .map(|_| simulate_extended_game(strategy_a, strategy_b))
                .collect();
            
            let base_game_a_wins = base_game_results.iter().filter(|&(a, b)| a < b).count();
            let base_game_b_wins = base_game_results.iter().filter(|&(a, b)| b < a).count();
            let base_game_ties = base_game_results.iter().filter(|&(a, b)| a == b).count();
            
            let extended_game_a_wins = extended_game_results.iter().filter(|&result| *result).count();
            let extended_game_b_wins = NUM_SIMULATIONS - extended_game_a_wins;
            
            results[format!("Strategy_{}_vs_Strategy_{}", i, j)] = json!({
                "base_game": {
                    "player_a_wins": base_game_a_wins,
                    "player_b_wins": base_game_b_wins,
                    "ties": base_game_ties,
                    "player_a_win_percentage": (base_game_a_wins as f64 / NUM_SIMULATIONS as f64) * 100.0,
                    "player_b_win_percentage": (base_game_b_wins as f64 / NUM_SIMULATIONS as f64) * 100.0,
                    "tie_percentage": (base_game_ties as f64 / NUM_SIMULATIONS as f64) * 100.0,
                },
                "extended_game": {
                    "player_a_wins": extended_game_a_wins,
                    "player_b_wins": extended_game_b_wins,
                    "player_a_win_percentage": (extended_game_a_wins as f64 / NUM_SIMULATIONS as f64) * 100.0,
                    "player_b_win_percentage": (extended_game_b_wins as f64 / NUM_SIMULATIONS as f64) * 100.0,
                }
            });
        }
    }

    results
}

fn main() {
    let start_time = Instant::now();
    
    println!("Running tournament with {} simulations for each matchup...", NUM_SIMULATIONS);
    
    let strategies = vec![
        CompositeStrategy::new(&[StrategyComponent::Random]),
        CompositeStrategy::new(&[StrategyComponent::HighestValue]),
        CompositeStrategy::new(&[StrategyComponent::HighestProbability]),
        CompositeStrategy::new(&[StrategyComponent::BalancedValue]),
        CompositeStrategy::new(&[StrategyComponent::Adaptive]),
        CompositeStrategy::new(&[StrategyComponent::LookAhead]),
        CompositeStrategy::new(&[StrategyComponent::LookAhead, StrategyComponent::HighestValue]),
        CompositeStrategy::new(&[StrategyComponent::Adaptive, StrategyComponent::HighestProbability]),
        // Add more composite strategies as needed
    ];
    
    let results = run_tournament(&strategies);
    
    let mut file = File::create("tournament_results.json").unwrap();
    file.write_all(serde_json::to_string_pretty(&results).unwrap().as_bytes()).unwrap();
    
    let duration = start_time.elapsed();
    println!("Tournament completed in {:?}", duration);
    println!("Results written to tournament_results.json");
}