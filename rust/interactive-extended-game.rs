use rand::Rng;
use std::io::{self, Write};

pub struct InteractiveExtendedGame {
    board: u16,
    subset_cache: Vec<Vec<u16>>,
    rng: rand::rngs::ThreadRng,
}

impl InteractiveExtendedGame {
    pub fn new() -> Self {
        let mut game = InteractiveExtendedGame {
            board: 0xFFF,
            subset_cache: vec![vec![]; 13],
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
        println!("Game starts. Initial board state:");
        self.display_board();
        
        let mut player_a_turn = true;
        loop {
            self.player_turn(player_a_turn);
            
            if self.board == 0 {
                println!("Player A wins!");
                break;
            } else if self.board == 0xFFF {
                println!("Player B wins!");
                break;
            }
            
            player_a_turn = !player_a_turn;
        }
    }

    fn player_turn(&mut self, is_player_a: bool) {
        println!("Player {}'s turn. Press Enter to roll the dice...", if is_player_a { "A" } else { "B" });
        io::stdin().read_line(&mut String::new()).expect("Failed to read line");

        let roll = self.roll_two_dice();
        println!("You rolled: {}", roll);

        let mask = 1 << (roll - 1);
        
        if is_player_a {
            if self.board & mask != 0 {
                println!("Do you want to turn off bit {}? (y/n):", roll);
                let mut choice = String::new();
                io::stdin().read_line(&mut choice).expect("Failed to read line");
                if choice.trim().to_lowercase() == "y" {
                    self.board &= !mask;
                    println!("Turned off bit {}", roll);
                } else {
                    println!("Searching for subset...");
                    for &num in &self.subset_cache[roll as usize] {
                        if self.board & (1 << (num - 1)) != 0 {
                            println!("Do you want to turn off bit {}? (y/n):", num);
                            let mut sub_choice = String::new();
                            io::stdin().read_line(&mut sub_choice).expect("Failed to read line");
                            if sub_choice.trim().to_lowercase() == "y" {
                                self.board &= !(1 << (num - 1));
                                println!("Turned off bit {}", num);
                                break;
                            }
                        }
                    }
                }
            } else {
                println!("Bit {} is already off. No valid moves.", roll);
            }
        } else {
            if self.board & mask == 0 {
                println!("Do you want to turn on bit {}? (y/n):", roll);
                let mut choice = String::new();
                io::stdin().read_line(&mut choice).expect("Failed to read line");
                if choice.trim().to_lowercase() == "y" {
                    self.board |= mask;
                    println!("Turned on bit {}", roll);
                } else {
                    println!("Searching for subset...");
                    for &num in &self.subset_cache[roll as usize] {
                        if self.board & (1 << (num - 1)) == 0 {
                            println!("Do you want to turn on bit {}? (y/n):", num);
                            let mut sub_choice = String::new();
                            io::stdin().read_line(&mut sub_choice).expect("Failed to read line");
                            if sub_choice.trim().to_lowercase() == "y" {
                                self.board |= 1 << (num - 1);
                                println!("Turned on bit {}", num);
                                break;
                            }
                        }
                    }
                }
            } else {
                println!("Bit {} is already on. No valid moves.", roll);
            }
        }

        println!("Current board:");
        self.display_board();
    }
}