#pragma once
#include <cstdint>

class CRC32 {
public:
	CRC32() { buildTable(); };
	~CRC32() {};
	int32_t ComputeCrc32(uint32_t w, uint8_t b);
	uint32_t BlockChecksum(uint8_t* buffer, int offset, int count, uint32_t seed = 0xffffffff);

private:
	uint32_t crc32Table[256];
	const uint32_t dwPolynomial = 0xedb88320UL;

	void buildTable();
};