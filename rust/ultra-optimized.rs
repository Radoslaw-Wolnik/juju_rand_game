use std::arch::x86_64::*;
use std::mem::MaybeUninit;

const ROUNDS: usize = 5;
const NUM_BITS: usize = 12;
const FULL_BOARD: u16 = 0xFFF;

#[repr(align(64))]
struct UltraOptimizedGame {
    boards: [u16; 2],
    scores: [u32; 2],
    subset_cache: [u16; 13],
    xorshift_state: [u64; 4],
}

impl UltraOptimizedGame {
    fn new(seed: u64) -> Self {
        let mut game = UltraOptimizedGame {
            boards: [FULL_BOARD; 2],
            scores: [0; 2],
            subset_cache: [0; 13],
            xorshift_state: [0; 4],
        };
        game.init_subset_cache();
        game.init_xorshift_state(seed);
        game
    }

    fn init_subset_cache(&mut self) {
        for i in 2..=12 {
            self.subset_cache[i] = (1 << i) - 1;
        }
    }

    fn init_xorshift_state(&mut self, seed: u64) {
        let mut state = [seed, seed ^ 0x1234567890ABCDEF, seed ^ 0xFEDCBA9876543210, seed ^ 0xAAAAAAAA55555555];
        for _ in 0..16 {
            self.xorshift128plus(&mut state);
        }
        self.xorshift_state = state;
    }

    #[inline(always)]
    fn xorshift128plus(&mut self) -> u64 {
        let x = self.xorshift_state[0];
        let y = self.xorshift_state[1];
        self.xorshift_state[0] = self.xorshift_state[2];
        self.xorshift_state[1] = self.xorshift_state[3];
        let mut x = x ^ (x << 23);
        x ^= x >> 17;
        x ^= y ^ (y >> 26);
        self.xorshift_state[2] = x;
        self.xorshift_state[3] = y;
        x.wrapping_add(y)
    }

    #[inline(always)]
    fn roll_dice(&mut self) -> u8 {
        ((self.xorshift128plus() & 0x7) % 6 + (self.xorshift128plus() & 0x38) % 6 + 2) as u8
    }

    #[inline(always)]
    fn generate_move(&self, board: u16, roll: u8) -> u16 {
        let direct_move = board & (1 << (roll - 1));
        if direct_move != 0 {
            return direct_move;
        }
        let subset = self.subset_cache[roll as usize] & board;
        if subset != 0 {
            unsafe { _blsi_u32(subset as u32) as u16 }
        } else {
            0
        }
    }

    fn play_turn(&mut self, player: usize) -> u16 {
        let initial_board = self.boards[player];
        while self.boards[player] != 0 {
            let roll = self.roll_dice();
            let move_ = self.generate_move(self.boards[player], roll);
            if move_ == 0 {
                break;
            }
            self.boards[player] &= !move_;
        }
        unsafe {
            _popcnt32(initial_board as i32) as u16 - _popcnt32(self.boards[player] as i32) as u16
        }
    }

    pub fn play(&mut self) {
        for _ in 0..ROUNDS {
            for player in 0..2 {
                self.boards[player] = FULL_BOARD;
                self.scores[player] += self.play_turn(player) as u32;
            }
        }
    }

    pub fn print_results(&self) {
        println!("Final scores: Player A = {}, Player B = {}", self.scores[0], self.scores[1]);
        println!("Winner: Player {}", if self.scores[0] < self.scores[1] { "A" } else { "B" });
    }
}

fn main() {
    let seed = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .unwrap()
        .as_nanos() as u64;
    
    let mut game = UltraOptimizedGame::new(seed);
    game.play();
    game.print_results();
}