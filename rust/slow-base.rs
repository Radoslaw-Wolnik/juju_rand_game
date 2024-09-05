// slow_base_game.rs
use rand::Rng;
use std::collections::HashSet;
use std::fs::File;
use std::io::Write;

const FULL_BOARD: u16 = 0xFFF;

pub struct SlowBaseGame {
    board: u16,
    subset_cache: Vec<Vec<u16>>,
    scores: [u32; 2],
    rng: rand::rngs::ThreadRng,
    log_file: File,
}

impl SlowBaseGame {
    pub fn new() -> Self {
        let mut game = SlowBaseGame {
            board: FULL_BOARD,
            subset_cache: vec![vec![]; 13],
            scores: [0; 2],
            rng: rand::thread_rng(),
            log_file: File::create("slow_base_game_log.txt").unwrap(),
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

    pub fn play(&mut self) {
        for round in 0..5 {
            writeln!(self.log_file, "Round {} begins:", round + 1).unwrap();
            for player in 0..2 {
                self.board = FULL_BOARD;
                let turn_score = self.player_turn(player);
                self.scores[player] += turn_score as u32;
                writeln!(self.log_file, "Player {} round score: {}", if player == 0 { "A" } else { "B" }, turn_score).unwrap();
                writeln!(self.log_file, "Current total: Player A = {}, Player B = {}\n", self.scores[0], self.scores[1]).unwrap();
            }
        }

        println!("Game over. Final scores: Player A = {}, Player B = {}", 
                 self.scores[0], self.scores[1]);
        println!("Winner: Player {}", if self.scores[0] < self.scores[1] { "A" } else { "B" });
        println!("Detailed log written to slow_base_game_log.txt");
    }

    fn player_turn(&mut self, player: usize) -> u16 {
        writeln!(self.log_file, "Player {}'s turn begins. Initial board: {:012b}", if player == 0 { "A" } else { "B" }, self.board).unwrap();
        loop {
            let roll = self.rng.gen_range(2..=12);
            writeln!(self.log_file, "  Rolled: {}", roll).unwrap();

            if self.board & (1 << (roll - 1)) != 0 {
                self.board &= !(1 << (roll - 1));
                writeln!(self.log_file, "  Turned off bit {}", roll).unwrap();
            } else {
                let mut subset_found = false;
                for &num in &self.subset_cache[roll as usize] {
                    if self.board & (1 << (num - 1)) != 0 {
                        self.board &= !(1 << (num - 1));
                        subset_found = true;
                        writeln!(self.log_file, "  Turned off bit {} (subset of {})", num, roll).unwrap();
                    }
                }
                if !subset_found {
                    let score = self.board.count_ones();
                    writeln!(self.log_file, "  No valid moves. Turn ends with score: {}", score).unwrap();
                    writeln!(self.log_file, "  Final board state: {:012b}", self.board).unwrap();
                    return score as u16;
                }
            }

            writeln!(self.log_file, "  Current board: {:012b}", self.board).unwrap();
            if self.board == 0 {
                writeln!(self.log_file, "