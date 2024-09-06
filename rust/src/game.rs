// File: src/game.rs

use crate::{GameState, Strategy, GameMode, OptimizationLevel, GameResult};
use crate::rng::Xoroshiro128Plus;
use rand::distributions::{Distribution, WeightedIndex};
use rand::Rng;
use rand::rngs::ThreadRng;

const FULL_BOARD: u16 = 0xFFF;

pub struct Game {
    rng: Box<dyn rand::RngCore>,
    weighted_dist: WeightedIndex<u32>,
    subset_cache: [u16; 13],
    mode: GameMode,
    optimization: OptimizationLevel,
}

impl Game {
    pub fn new(mode: GameMode, optimization: OptimizationLevel, seed: u64) -> Self {
        let rng: Box<dyn rand::RngCore> = match optimization {
            OptimizationLevel::Standard | OptimizationLevel::FastPrecise => Box::new(ThreadRng::default()),
            OptimizationLevel::Fast => Box::new(Xoroshiro128Plus::new(seed)),
            OptimizationLevel::UltraOptimized => Box::new(Xoroshiro128Plus::new(seed)), // You might want to implement an even faster RNG for this
        };

        let mut game = Game {
            rng,
            weighted_dist: WeightedIndex::new(&[1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1]).unwrap(),
            subset_cache: [0; 13],
            mode,
            optimization,
        };
        game.init_subset_cache();
        game
    }

    fn init_subset_cache(&mut self) {
        for i in 2..=12 {
            self.subset_cache[i] = (1 << i) - 1;
        }
    }

    fn roll_dice(&mut self) -> u8 {
        match self.optimization {
            OptimizationLevel::Standard | OptimizationLevel::FastPrecise => {
                self.rng.gen_range(1..=6) + self.rng.gen_range(1..=6)
            },
            OptimizationLevel::Fast | OptimizationLevel::UltraOptimized => {
                (self.weighted_dist.sample(&mut self.rng) + 2) as u8
            },
        }
    }

    fn generate_move(&self, board: u16, roll: u8) -> u16 {
        let direct_move = board & (1 << (roll - 1));
        if direct_move != 0 {
            return direct_move;
        }
        let subset = self.subset_cache[roll as usize] & board;
        if subset != 0 {
            subset & (!subset + 1) // BLSI instruction equivalent
        } else {
            0
        }
    }

    pub fn play_game(&mut self, strategies: &[&dyn Strategy]) -> GameResult {
        let mut state = GameState { board: FULL_BOARD, score: 0, round: 0 };
        let mut scores = vec![0; strategies.len()];
        let mut current_player = 0;

        loop {
            let roll = self.roll_dice();
            let move_bits = strategies[current_player].choose_move(&state, roll);

            match self.mode {
                GameMode::Base => {
                    if move_bits == 0 {
                        scores[current_player] += state.board.count_ones() as u32;
                        state.round += 1;
                        if state.round >= 5 {
                            break;
                        }
                        state.board = FULL_BOARD;
                        current_player = (current_player + 1) % strategies.len();
                    } else {
                        state.board &= !move_bits;
                    }
                },
                GameMode::Extended => {
                    if move_bits == 0 {
                        current_player = (current_player + 1) % strategies.len();
                    } else {
                        state.board = if current_player == 0 { state.board & !move_bits } else { state.board | move_bits };
                        if state.board == 0 || state.board == FULL_BOARD {
                            break;
                        }
                    }
                },
            }

            state.round += 1;
        }

        let winner = match self.mode {
            GameMode::Base => scores.iter().enumerate().min_by_key(|&(_, &score)| score).unwrap().0,
            GameMode::Extended => if state.board == 0 { 0 } else { 1 },
        };

        GameResult {
            winner,
            scores,
            num_rounds: state.round,
        }
    }
}