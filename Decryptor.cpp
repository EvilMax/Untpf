#include "Decryptor.h"

Decryptor::Decryptor() {

}

Decryptor::~Decryptor() {

}

unsigned char Decryptor::MagicByte() {
	uint16_t t = (uint16_t)((uint16_t)(keys[2] & 0xffff) | 2);
	return (uint8_t)(((t * (t ^ 1)) >> 8) & 0xff);
}

