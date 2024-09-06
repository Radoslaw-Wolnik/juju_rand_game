// File: src/multithreaded_simulator.rs

use std::sync::{Arc, Mutex};
use std::thread;
use crate::{GameMode, OptimizationLevel, Strategy, GameResult};
use crate::game::Game;
use rand::Rng;

pub struct MultithreadedSimulator;

impl MultithreadedSimulator {
    pub fn run_simulations(
        mode: GameMode,
        optimization: OptimizationLevel,
        strategies: &[Arc<dyn Strategy>],
        total_games: usize,
        num_threads: usize
    ) -> Vec<(String, f64)> {
        let games_per_thread = total_games / num_threads;
        let results = Arc::new(Mutex::new(Vec::new()));
        let games_completed = Arc::new(Mutex::new(0));

        let mut handles = vec![];

        for _ in 0..num_threads {
            let results = Arc::clone(&results);
            let games_completed = Arc::clone(&games_completed);
            let strategies = strategies.to_vec();
            let handle = thread::spawn(move || {
                let mut rng = rand::thread_rng();
                for _ in 0..games_per_thread {
                    let mut game = Game::new(mode, optimization, seed);
                    let game_result = game.play_game(&[strategies[0].as_ref(), strategies[1].as_ref()]);
                    
                    {
                        let mut results = results.lock().unwrap();
                        results.push(game_result);
                    }
                    
                    {
                        let mut completed = games_completed.lock().unwrap();
                        *completed += 1;
                    }
                }
            });
            handles.push(handle);
        }

        for handle in handles {
            handle.join().unwrap();
        }

        let final_results = results.lock().unwrap();
        Self::analyze_results(&final_results, strategies)
    }

    fn analyze_results(results: &[GameResult], strategies: &[Arc<dyn Strategy>]) -> Vec<(String, f64)> {
        let mut wins = vec![0; strategies.len()];
        for result in results {
            wins[result.winner] += 1;
        }
        
        strategies.iter().enumerate()
            .map(|(i, s)| (s.name().to_string(), wins[i] as f64 / results.len() as f64))
            .collect()
    }
}