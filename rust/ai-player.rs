pub struct AIPlayer;

impl AIPlayer {
    pub fn make_move(board: &mut u16, roll: u16, is_player_a: bool) -> bool {
        let mask = 1 << (roll - 1);
        
        if is_player_a {
            if *board & mask != 0 {
                *board &= !mask;
                true
            } else {
                for i in 1..roll {
                    if *board & (1 << (i - 1)) != 0 && (roll - i <= 6) {
                        *board &= !(1 << (i - 1));
                        return true;
                    }
                }
                false
            }
        } else {
            if *board & mask == 0 {
                *board |= mask;
                true
            } else {
                for i in 1..roll {
                    if *board & (1 << (i - 1)) == 0 && (roll - i <= 6) {
                        *board |= 1 << (i - 1);
                        return true;
                    }
                }
                false
            }
        }
    }
}