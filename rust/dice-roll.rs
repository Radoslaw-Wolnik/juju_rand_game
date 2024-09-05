// Add this method to all Rust game structs
fn roll_two_dice(&mut self) -> u16 {
    self.rng.gen_range(1..=6) + self.rng.gen_range(1..=6)
}

// Replace the random number generation in all game structs with:
let roll = self.roll_two_dice();