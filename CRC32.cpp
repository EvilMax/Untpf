#include "CRC32.h"

void CRC32::buildTable() {
	uint32_t dwCrc;
	uint8_t i = 0;
	do {
		dwCrc = i;
		for (unsigned char j = 8; j > 0; j--) {
			if ((dwCrc & 1) == 1) {
				dwCrc = (dwCrc >> 1) ^ dwPolynomial;
			}
			else {
				dwCrc >>= 1;
			}
		}
		crc32Table[i] = dwCrc;
		i++;
	} while (i != 0);
}

int32_t CRC32::ComputeCrc32(uint32_t w, uint8_t b) {
	return (int32_t)(crc32Table[(w ^ b) & 0xFF] ^ (w >> 8));
}

uint32_t CRC32::BlockChecksum(uint8_t* buffer, int offset, int count, uint32_t seed) {
	uint32_t crc = seed;
	for (int i = offset; i < count; i++) {
		crc = (crc >> 8) ^ crc32Table[(buffer[i] ^ (crc & 0xff))];
	}
	return ~crc;
}

