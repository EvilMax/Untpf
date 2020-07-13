#pragma once

#include <cstdint>

class Decryptor {
public:
	Decryptor();
	~Decryptor();
private:
	uint32_t keys[4];

	unsigned char MagicByte();
};