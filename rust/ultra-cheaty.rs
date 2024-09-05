// ultra_cheaty_fast_game.rs
use rand::distributions::{Distribution, WeightedIndex};
use rand::Rng;
use std::collections::HashSet;

const FULL_BOARD: u16 = 0xFFF;

pub struct UltraCheatyFastGame {
    board: u16,
    subset_cache: Vec<Vec<u16>>,
    scores: [u32; 2],
    rng: rand::rngs::ThreadRng,
    weighted_dist: WeightedIndex<u32>,
}

impl UltraCheatyFastGame {
    pub fn new() -> Self {
        let mut game = UltraCheatyFastGame {
            board: FULL_BOARD,
            subset_cache: vec![vec![]; 13],
            scores: [0; 2],
            rng: rand::thread_rng(),
            weighted_dist: WeightedIndex::new(&[1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1]).unwrap(),
        };
        game.init_subset_cache();
        game
    }

    fn init_subset_cache(&mut self) {
        for i in 2..=12 {
            let mut subset = HashSet::new();
            for j in 1..i {
                if (1 << (j - 1)) & ((1 << i) - 1) != 0 {
                    subset.insert(j as u16);
                }
            }
            self.subset_cache[i] = subset.into_iter().collect();
        }
    }

    fn roll_weighted_dice(&mut self) -> u16 {
        (self.weighted_dist.sample(&mut self.rng) + 2) as u16
    }

    pub fn play(&mut self) {
        for round in 0..5 {
            for player in 0..2 {
                self.board = FULL_BOARD;
                let turn_score = self.player_turn(player);
                self.scores[player] += turn_score as u32;
            }
        }

        println!("Game over. Final scores: Player A = {}, Player B = {}", 
                 self.scores[0], self.scores[1]);
        println!("Winner: Player {}", if self.scores[0] < self.scores[1] { "A" } else { "B" });
    }

    fn player_turn(&mut self, _player: usize) -> u16 {
        loop {
            let roll = self.roll_weighted_dice();
            let mask = 1 << (roll - 1);

            if self.board & mask != 0 {
                self.board &= !mask;
            } else {
                let mut subset_found = false;
                for &num in &self.subset_cache[roll as usize] {
                    if self.board & (1 << (num - 1)) != 0 {
                        self.board &= !(1 << (num - 1));
                        subset_found = true;
                    }
                }
                if !subset_found {
                    return self.board.count_ones() as u16;
                }
            }

            if self.board == 0 {
                return 0;  // Perfect score
            }
        }
    }
}