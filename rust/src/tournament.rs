// File: src/tournament.rs

use crate::{GameMode, OptimizationLevel, Strategy, game::Game};
use std::collections::HashMap;
use rayon::prelude::*;
use rand::Rng;

pub fn run_simulation(mode: GameMode, optimization: OptimizationLevel, strategies: &[Box<dyn Strategy>], num_games: usize) -> HashMap<String, HashMap<String, f64>> {
    let mut results = HashMap::new();

    for (i, strategy1) in strategies.iter().enumerate() {
        for strategy2 in strategies.iter().skip(i + 1) {
            let (wins1, wins2) = (0..num_games).into_par_iter().map(|_| {
                let mut rng = rand::thread_rng();
                let seed = rng.gen();
                let mut game = Game::new(mode, optimization, seed);
                let game_result = game.play_game(&[strategy1.as_ref(), strategy2.as_ref()]);
                match game_result.winner {
                    0 => (1, 0),
                    1 => (0, 1),
                    _ => (0, 0),
                }
            }).reduce(|| (0, 0), |a, b| (a.0 + b.0, a.1 + b.1));

            let win_rate1 = wins1 as f64 / num_games as f64;
            let win_rate2 = wins2 as f64 / num_games as f64;

            results.entry(strategy1.name().to_string())
                .or_insert_with(HashMap::new)
                .insert(strategy2.name().to_string(), win_rate1);

            results.entry(strategy2.name().to_string())
                .or_insert_with(HashMap::new)
                .insert(strategy1.name().to_string(), win_rate2);
        }
    }

    results
}