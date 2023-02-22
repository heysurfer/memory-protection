namespace hs {
	constexpr uint32_t generate_seed()
	{
		constexpr uint32_t seed = 
		(static_cast<uint32_t>(__TIME__[0] - '0') * 600)
		+
		(static_cast<uint32_t>(__TIME__[0] - '0') * 5469)
		+
		(static_cast<uint32_t>(__TIME__[0] - '0') * 1473)
		+
		(static_cast<uint32_t>(__TIME__[0] - '0') * 34527);

		return seed;
	}
	constexpr uint32_t kPrime1 = 0x9E3779B1U;
	constexpr uint32_t kPrime2 = 0x85EBCA77U;
	constexpr uint32_t kPrime3 = 0xC2B2AE3DU;
	constexpr uint32_t kPrime4 = 0x27D4EB2FU;
	constexpr uint32_t kSeed = generate_seed();
	inline uint32_t RotateLeft(uint32_t x, uint32_t r)
	{
		return (x << r) | (x >> (32 - r));
	}
	inline uint32_t Round(uint32_t acc, uint32_t input)
	{
		acc += input * kPrime2;
		acc = RotateLeft(acc, 13);
		acc *= kPrime1;
		return acc;
	}
	inline uint32_t MergeRound(uint32_t acc, uint32_t val)
	{
		val = Round(0, val);
		acc ^= val;
		acc = acc * kPrime1 + kPrime4;
		return acc;
	}
	inline uint32_t FinalRound(uint32_t acc)
	{
		acc ^= acc >> 15;
		acc *= kPrime2;
		acc ^= acc >> 13;
		acc *= kPrime3;
		acc ^= acc >> 16;
		return acc;
	}
	uint32_t xxHash(const void* data, size_t len, uint32_t seed = kSeed)
	{
		const uint8_t* ptr = static_cast<const uint8_t*>(data);
		const uint8_t* end = ptr + len;
		uint32_t acc = seed + kPrime1 + kPrime2;
		uint32_t tmp;

		while (ptr + 4 <= end)
		{
			tmp = *reinterpret_cast<const uint32_t*>(ptr);
			acc = Round(acc, tmp);
			ptr += 4;
		}

		tmp = 0;
		switch (end - ptr) {
		case 3: tmp ^= static_cast<uint32_t>(ptr[2]) << 16; // fallthrough
		case 2: tmp ^= static_cast<uint32_t>(ptr[1]) << 8;  // fallthrough
		case 1: tmp ^= static_cast<uint32_t>(ptr[0]);
			acc = Round(acc, tmp);
		}

		acc = MergeRound(acc, len);
		return FinalRound(acc);
	}
	template<typename T>
	uint32_t xxHash(const T& value, uint32_t seed = kSeed)
	{
		return xxHash(&value, sizeof(value), seed);
	}
	constexpr uint64_t val_64_const = 0xcbf29ce484222325u;
	constexpr uint64_t prime_64_const = 0x100000001b3u;
	inline constexpr uint64_t hash64(const char* const str, const uint64_t value = val_64_const) noexcept {
			return (str[0] == '\0') ? value : hash64(&str[1], (value ^ uint64_t(str[0])) * prime_64_const);
		}
	
}
