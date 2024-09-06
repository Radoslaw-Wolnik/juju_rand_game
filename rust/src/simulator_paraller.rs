// File: src/parallel_simulator.rs

use rayon::prelude::*;
use crate::{GameMode, OptimizationLevel, Strategy, GameResult};
use crate::game::Game;
use std::sync::Arc;
use rand::Rng;

pub struct ParallelSimulator;

impl ParallelSimulator {
    pub fn run_simulations(
        mode: GameMode,
        optimization: OptimizationLevel,
        strategies: &[Arc<dyn Strategy>],
        total_games: usize
    ) -> Vec<(String, f64)> {
        let results: Vec<GameResult> = (0..total_games)
            .into_par_iter()
            .map(|_| {
                let mut rng = rand::thread_rng();
                let seed = rng.gen();
                let mut game = Game::new(mode, optimization, seed);
                game.play_game(&[strategies[0].as_ref(), strategies[1].as_ref()])
            })
            .collect();

        Self::analyze_results(&results, strategies)
    }

    fn analyze_results(results: &[GameResult], strategies: &[Arc<dyn Strategy>]) -> Vec<(String, f64)> {
        let wins: Vec<usize> = (0..strategies.len())
            .map(|i| results.iter().filter(|r| r.winner == i).count())
            .collect();
        
        strategies.iter().enumerate()
            .map(|(i, s)| (s.name().to_string(), wins[i] as f64 / results.len() as f64))
            .collect()
    }
}