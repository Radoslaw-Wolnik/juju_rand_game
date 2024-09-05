mod interactive_base_game;
mod interactive_extended_game;
mod multithreaded_simulations;
// ... (other modules)

use interactive_base_game::InteractiveBaseGame;
use interactive_extended_game::InteractiveExtendedGame;
use multithreaded_simulations::MultithreadedSimulations;
// ... (other uses)

fn main() {
    loop {
        println!("\nChoose a game mode:");
        println!("1. Interactive Base Game");
        println!("2. Interactive Extended Game");
        println!("3. AI Game (Base Rules)");
        println!("4. AI Game (Extended Rules)");
        println!("5. Run Multithreaded Simulations");
        println!("6. Exit");

        let choice: u32 = get_user_input("Enter your choice (1-6): ");

        match choice {
            1 => {
                let mut game = InteractiveBaseGame::new();
                game.play();
            }
            2 => {
                let mut game = InteractiveExtendedGame::new();
                game.play();
            }
            3 => {
                // Implement AI game with base rules
            }
            4 => {
                // Implement AI game with extended rules
            }
            5 => {
                run_multithreaded_simulations();
            }
            6 => {
                println!("Thanks for playing. Goodbye!");
                break;
            }
            _ => println!("Invalid choice. Please try again."),
        }
    }
}

fn get_user_input(prompt: &str) -> u32 {
    loop {
        println!("{}", prompt);
        let mut input = String::new();
        std::io::stdin().read_line(&mut input).expect("Failed to read line");
        match input.trim().parse() {
            Ok(num) => return num,
            Err(_) => println!("Please enter a valid number."),
        }
    }
}