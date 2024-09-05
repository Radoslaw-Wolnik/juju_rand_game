// ultra_fast_base_game.rs
use std::sync::atomic::{AtomicU16, Ordering};
use std::time::{SystemTime, UNIX_EPOCH};

const FULL_BOARD: u16 = 0xFFF;

struct FastRandom {
    state: [u64; 2],
}

impl FastRandom {
    fn new(seed1: u64, seed2: u64) -> Self {
        FastRandom { state: [seed1, seed2] }
    }

    fn next(&mut self) -> u64 {
        let s0 = self.state[0];
        let mut s1 = self.state[1];
        let result = s0.wrapping_add(s1);

        s1 ^= s0;
        self.state[0] = s0.rotate_left(55) ^ s1 ^ (s1 << 14);
        self.state[1] = s1.rotate_left(36);

        result
    }

    fn roll_dice(&mut self) -> u16 {
        (self.next() % 11 + 2) as u16
    }
}

pub struct UltraFastBaseGame {
    board: AtomicU16,
    subset_cache: [u16; 13],
    scores: [u32; 2],
}

impl UltraFastBaseGame {
    pub fn new() -> Self {
        let mut game = UltraFastBaseGame {
            board: AtomicU16::new(FULL_BOARD),
            subset_cache: [0; 13],
            scores: [0; 2],
        };
        game.init_subset_cache();
        game
    }

    fn init_subset_cache(&mut self) {
        for i in 2..=12 {
            self.subset_cache[i] = (1 << i) - 1;
        }
    }

    pub fn play(&mut self) {
        let seed = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_nanos() as u64;
        let mut rng = FastRandom::new(seed, seed.wrapping_add(1));

        for _ in 0..5 {
            for player in 0..2 {
                self.board.store(FULL_BOARD, Ordering::Relaxed);
                let turn_score = self.player_turn(player, &mut rng);
                self.scores[player] += turn_score as u32;
            }
        }

        println!("Game over. Final scores: Player A = {}, Player B = {}", 
                 self.scores[0], self.scores[1]);
        println!("Winner: Player {}", if self.scores[0] < self.scores[1] { "A" } else { "B" });
    }

    fn player_turn(&self, _player: usize, rng: &mut FastRandom) -> u16 {
        loop {
            let roll = rng.roll_dice();
            let mask = 1 << (roll - 1);
            let subset = self.subset_cache[roll as usize];

            let result = self.board.fetch_update(Ordering::Release, Ordering::Relaxed, |old_board| {
                if old_board & mask != 0 {
                    Some(old_board & !mask)
                } else if old_board & subset == subset {
                    Some(old_board & !subset)
                } else {
                    None
                }
            });

            match result {
                Ok(new_board) => {
                    if new_board == 0 {
                        return 0;  // Perfect score
                    }
                },
                Err(old_board) => {
                    return old_board.count_ones() as u16;  // Turn ends, return score
                }
            }

            std::hint::spin_loop();
        }
    }
}