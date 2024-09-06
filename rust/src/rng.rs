
pub struct Xoroshiro128Plus {
    s: [u64; 2],
}

impl Xoroshiro128Plus {
    pub fn new(seed: u64) -> Self {
        let mut rng = Xoroshiro128Plus { s: [0; 2] };
        rng.seed(seed);
        rng
    }

    pub fn seed(&mut self, seed: u64) {
        let mut s = [0u8; 16];
        for i in 0..8 {
            s[i] = (seed >> (i * 8)) as u8;
        }
        self.s[0] = u64::from_le_bytes([s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7]]);
        self.s[1] = u64::from_le_bytes([s[8], s[9], s[10], s[11], s[12], s[13], s[14], s[15]]);
    }

    pub fn next_u64(&mut self) -> u64 {
        let s0 = self.s[0];
        let mut s1 = self.s[1];
        let result = s0.wrapping_add(s1);

        s1 ^= s0;
        self.s[0] = s0.rotate_left(24) ^ s1 ^ (s1 << 16);
        self.s[1] = s1.rotate_left(37);

        result
    }

    pub fn next_u32(&mut self) -> u32 {
        self.next_u64() as u32
    }
}