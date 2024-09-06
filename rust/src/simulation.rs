// File: src/simulation.rs

use crate::{GameMode, OptimizationLevel, Strategy, GameResult};
use crate::multithreaded_simulator::MultithreadedSimulator;
use crate::parallel_simulator::ParallelSimulator;
use crate::distributed_simulator::DistributedSimulator;
use std::sync::Arc;

pub enum SimulatorType {
    Multithreaded,
    Parallel,
    Distributed,
}

pub struct SimulationConfig {
    pub mode: GameMode,
    pub optimization: OptimizationLevel,
    pub strategies: Vec<Arc<dyn Strategy>>,
    pub num_games: usize,
    pub simulator_type: SimulatorType,
}

pub fn run_simulation(config: SimulationConfig) -> Vec<(String, f64)> {
    match config.simulator_type {
        SimulatorType::Multithreaded => {
            MultithreadedSimulator::run_simulations(
                config.mode,
                config.optimization,
                &config.strategies,
                config.num_games,
                num_cpus::get()
            )
        },
        SimulatorType::Parallel => {
            ParallelSimulator::run_simulations(
                config.mode,
                config.optimization,
                &config.strategies,
                config.num_games
            )
        },
        SimulatorType::Distributed => {
            let simulator = DistributedSimulator::new();
            simulator.run_simulations(
                config.mode,
                config.optimization,
                &config.strategies,
                config.num_games
            )
        },
    }
}