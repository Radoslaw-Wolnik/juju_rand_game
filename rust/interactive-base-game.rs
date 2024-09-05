use rand::Rng;
use std::io::{self, Write};

pub struct InteractiveBaseGame {
    board: u16,
    subset_cache: Vec<Vec<u16>>,
    scores: [u32; 2],
    rng: rand::rngs::ThreadRng,
}

impl InteractiveBaseGame {
    pub fn new() -> Self {
        let mut game = InteractiveBaseGame {
            board: 0xFFF,
            subset_cache: vec![vec![]; 13],
            scores: [0; 2],
            rng: rand::thread_rng(),
        };
        game.init_subset_cache();
        game
    }

    fn init_subset_cache(&mut self) {
        for i in 2..=12 {
            for j in 1..i {
                if (1 << (j - 1)) & ((1 << i) - 1) != 0 {
                    self.subset_cache[i].push(j as u16);
                }
            }
        }
    }

    fn roll_two_dice(&mut self) -> u16 {
        self.rng.gen_range(1..=6) + self.rng.gen_range(1..=6)
    }

    fn display_board(&self) {
        println!("+---+---+---+---+---+---+---+---+---+---+---+---+");
        print!("| ");
        for i in (0..12).rev() {
            print!("{} | ", if self.board & (1 << i) != 0 { "O" } else { "X" });
        }
        println!("\n+---+---+---+---+---+---+---+---+---+---+---+---+");
        println!("  12  11  10   9   8   7   6   5   4   3   2   1  ");
    }

    pub fn play(&mut self) {
        println!("Enter the number of rounds to play:");
        let mut input = String::new();
        io::stdin().read_line(&mut input).expect("Failed to read line");
        let rounds: i32 = input.trim().parse().expect("Please enter a number");

        for round in 0..rounds {
            println!("Round {} begins:", round + 1);
            for player in 0..2 {
                self.board = 0xFFF;
                let turn_score = self.player_turn(player);
                self.scores[player] += turn_score as u32;
                println!("Player {} round score: {}", if player == 0 { "A" } else { "B" }, turn_score);
                println!("Current total: Player A = {}, Player B = {}\n", self.scores[0], self.scores[1]);
            }
        }

        println!("Game over. Final scores: Player A = {}, Player B = {}", self.scores[0], self.scores[1]);
        println!("Winner: Player {}", if self.scores[0] < self.scores[1] { "A" } else { "B" });
    }

    fn player_turn(&mut self, player: usize) -> u16 {
        println!("Player {}'s turn begins. Initial board:", if player == 0 { "A" } else { "B" });
        self.display_board();
        
        loop {
            println!("Press Enter to roll the dice...");
            io::stdin().read_line(&mut String::new()).expect("Failed to read line");

            let roll = self.roll_two_dice();
            println!("You rolled: {}", roll);

            if self.board & (1 << (roll - 1)) != 0 {
                println!("Do you want to turn off bit {}? (y/n):", roll);
                let mut choice = String::new();
                io::stdin().read_line(&mut choice).expect("Failed to read line");
                if choice.trim().to_lowercase() == "y" {
                    self.board &= !(1 << (roll - 1));
                    println!("Turned off bit {}", roll);
                } else {
                    println!("Searching for subset...");
                    let mut subset_found = false;
                    for &num in &self.subset_cache[roll as usize] {
                        if self.board & (1 << (num - 1)) != 0 {
                            println!("Do you want to turn off bit {}? (y/n):", num);
                            let mut sub_choice = String::new();
                            io::stdin().read_line(&mut sub_choice).expect("Failed to read line");
                            if sub_choice.trim().to_lowercase() == "y" {
                                self.board &= !(1 << (num - 1));
                                subset_found = true;
                                println!("Turned off bit {}", num);
                                break;
                            }
                        }
                    }
                    if !subset_found {
                        let score = self.board.count_ones();
                        println!("No valid moves. Turn ends with score: {}", score);
                        println!("Final board state:");
                        self.display_board();
                        return score as u16;
                    }
                }
            } else {
                println!("Bit {} is already off. Searching for subset...", roll);
                let mut subset_found = false;
                for &num in &self.subset_cache[roll as usize] {
                    if self.board & (1 << (num - 1)) != 0 {
                        println!("Do you want to turn off bit {}? (y/n):", num);
                        let mut choice = String::new();
                        io::stdin().read_line(&mut choice).expect("Failed to read line");
                        if choice.trim().to_lowercase() == "y" {
                            self.board &= !(1 << (num - 1));
                            subset_found = true;
                            println!("Turned off bit {}", num);
                            break;
                        }
                    }
                }
                if !subset_found {
                    let score = self.board.count_ones();
                    println!("No valid moves. Turn ends with score: {}", score);
                    println!("Final board state:");
                    self.display_board();
                    return score as u16;
                }
            }

            println!("Current board:");
            self.display_board();

            if self.board == 0 {
                println!("All bits turned off! Perfect score of 0.");
                return 0;
            }
        }
    }
}