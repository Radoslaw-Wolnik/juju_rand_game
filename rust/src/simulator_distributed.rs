// File: src/distributed_simulator.rs

use mpi::traits::*;
use mpi::topology::Communicator;
use crate::{GameMode, OptimizationLevel, Strategy, GameResult};
use crate::game::Game;
use std::sync::Arc;
use rand::Rng;

pub struct DistributedSimulator {
    world: mpi::topology::SystemCommunicator,
}

impl DistributedSimulator {
    pub fn new() -> Self {
        let universe = mpi::initialize().unwrap();
        let world = universe.world();
        DistributedSimulator { world }
    }

    pub fn run_simulations(
        &self,
        mode: GameMode,
        optimization: OptimizationLevel,
        strategies: &[Arc<dyn Strategy>],
        total_simulations: usize
    ) -> Vec<(String, f64)> {
        let world_size = self.world.size() as usize;
        let world_rank = self.world.rank() as usize;
        let local_simulations = total_simulations / world_size;

        let mut rng = rand::thread_rng();
        let local_results: Vec<GameResult> = (0..local_simulations)
            .map(|_| {
                let seed = rng.gen();
                let mut game = Game::new(mode, optimization, seed);
                game.play_game(&[strategies[0].as_ref(), strategies[1].as_ref()])
            })
            .collect();

        let mut global_results = if world_rank == 0 {
            vec![GameResult::default(); total_simulations]
        } else {
            vec![]
        };

        self.world.gather_into_root(&local_results[..], &mut global_results[..]);

        if world_rank == 0 {
            Self::analyze_results(&global_results, strategies)
        } else {
            vec![]
        }
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