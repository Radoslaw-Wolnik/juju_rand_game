// main.rs
// ... (previous imports remain the same)

fn main() {
    loop {
        explain_rules();

        println!("\nPlease choose a game mode:");
        println!("1. Base Game");
        println!("2. Extra Game");
        println!("3. Exit");

        let mode_choice = get_int_input("Enter your choice (1-3): ");

        if mode_choice == 3 {
            println!("Thanks for playing. Goodbye!");
            break;
        }

        if mode_choice != 1 && mode_choice != 2 {
            println!("Invalid choice. Please try again.");
            continue;
        }

        println!("\nChoose an algorithm speed:");
        println!("1. Ultra-fast (minimal output)");
        println!("2. Fast (balanced speed and output)");
        println!("3. Slow (detailed output to file)");
        println!("4. Proper Random (cryptographically secure, same as Fast)");

        let speed_choice = get_int_input("Enter your choice (1-4): ");

        if speed_choice < 1 || speed_choice > 4 {
            println!("Invalid choice. Please try again.");
            continue;
        }

        match (mode_choice, speed_choice) {
            (1, 1) => UltraFastBaseGame::new().play(),
            (1, 2) | (1, 4) => FastBaseGame::new().play(),
            (1, 3) => SlowBaseGame::new().play(),
            (2, 1) => UltraFastGame::new().play(),
            (2, 2) | (2, 3) | (2, 4) => FastGame::new().play(),
            _ => unreachable!(),
        }

        // ... (rest of the code remains the same)
    }
}