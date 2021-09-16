extern crate std;

/// This is a RNG called WELL512, designed by the designers of Mersenne, developed 10 years later,
/// and an all around better choice for games. The code is put in the public domain
/// by Dr. Chris Lomont. He claims this implementation is 40% faster than Mersenne,
/// does not suffer from poor diffusion and trapping when the state contains many 0
/// bits, and is clearly a lot simpler code. It has a period of 2^512; a PC takes
/// over 10^100 years to cycle through the states, so it is large enough.
/// Here is a paper overviewing PRNGs where I found the WELL512 implementation.
/// http://www.lomont.org/Math/Papers/2008/Lomont_PRNG_2008.pdf
/// So - faster, simpler, created by the same designers 10 years later, and produces
/// better numbers than Mersenne. How can you go wrong? :)
/// 
/// For more information please see http://stackoverflow.com/a/1227137
/// and http://stackoverflow.com/questions/1046714/what-is-a-good-random-number-generator-for-a-game
pub struct Rng {
	index : usize,
	state : [u32; 16],
	// Rngs shouldn't be copyable, but this introduce problems in the constructor, hrm...
	// _marker: std::marker::NoCopy
}

impl Rng {
	/// Creates a new generator based on a seed
	pub fn new(seed:u32) -> Rng {
		let mut r = Rng {
			index: 0,
			// doesn't seem to be supported...
			// http://stackoverflow.com/questions/26757355/how-do-i-collect-into-an-array
			// state: range(0, 16).map(|i| i*seed).collect::<Vec<u32>>().slice(0,16)
			state: [0;16]
		};
		for i in range(0,16) {
			r.state[i] = seed * i as u32;
		}
		r
	}

	// seems to be that the the time is hard to import on windows, lets keep it hidden for now...
	// fn new_ns() -> Rng {
	// 	let ct = time::time::get_time();
	// 	return Rng::new(ct.nsep as u32);
	// }

	/// Generate a new random number
	pub fn next(&mut self) -> u32 {
		let mut a = self.state[self.index];
		let mut c = self.state[(self.index + 13) & 15];
		let b = a ^ c ^ (a << 16) ^ (c << 15);
		c = self.state[(self.index + 9) & 15];
		c ^= c >> 11;
		self.state[self.index] = b ^ c;
		a = self.state[self.index];
		let d = a ^ ((a << 5) & 0xDA442D24);
		self.index = (self.index + 15) & 15;
		a = self.state[self.index];
		self.state[self.index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);
		self.state[self.index]
	}

	/// Generate a new random float between 0 and 1
	pub fn next01(&mut self) -> f32 {
		self.next() as f32 / std::u32::MAX as f32
	}
}
