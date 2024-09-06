// File: src/lib.rs

pub mod game;
pub mod strategies;
pub mod tournament;
pub mod simulation;
pub mod rng;

pub use game::{GameMode, OptimizationLevel};
pub use strategies::Strategy;
pub use tournament::run_simulation;

use serde::{Serialize, Deserialize};
use std::collections::HashMap;

#[derive(Clone, Copy, Debug, Serialize, Deserialize)]
pub struct GameState {
    pub board: u16,
    pub score: u32,
    pub round: u8,
}

pub trait Strategy: Send + Sync {
    fn choose_move(&self, state: &GameState, roll: u8) -> u16;
    fn name(&self) -> &'static str;
}

#[derive(Clone, Copy, Debug, Serialize, Deserialize)]
pub enum GameMode {
    Base,
    Extended,
}

#[derive(Clone, Copy, Debug, Serialize, Deserialize)]
pub enum OptimizationLevel {
    Standard,
    FastPrecise,
    Fast,
    UltraOptimized,
}

#[derive(Serialize, Deserialize)]
pub struct SimulationConfig {
    pub num_games: usize,
    pub game_mode: GameMode,
    pub optimization_level: OptimizationLevel,
    pub strategies: Vec<String>,
    pub seed: u64,
}

#[derive(Serialize, Deserialize)]
pub struct GameResult {
    pub winner: usize,
    pub scores: Vec<u32>,
    pub num_rounds: u8,
}

#[derive(Serialize, Deserialize)]
pub struct SimulationResult {
    pub config: SimulationConfig,
    pub results: HashMap<String, Vec<GameResult>>,
}
