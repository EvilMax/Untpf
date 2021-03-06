#pragma once

#include <string>

using namespace std;

const char pw[] = {
	0x73, 0x2A, 0x63, 0x7D, 0x5F, 0x0A, 0xA6, 0xBD,
	0x7D, 0x65, 0x7E, 0x67, 0x61, 0x2A, 0x7F, 0x7F,
	0x74, 0x61, 0x67, 0x5B, 0x60, 0x70, 0x45, 0x74,
	0x5C, 0x22, 0x74, 0x5D, 0x6E, 0x6A, 0x73, 0x41,
	0x77, 0x6E, 0x46, 0x47, 0x77, 0x49, 0x0C, 0x4B,
	0x46, 0x6F, '\0'
};

class TpfFile
{
public:
	TpfFile();
	TpfFile(string destination);
	~TpfFile();

	void SetFile(string name);
	void Unpack();

private:
	string name;
	string destination;
	bool loaded;
	bool xored;
	uint8_t * data;
	uint32_t dataLen;
	uint32_t bufferLen;

	void ReadFile();
	void UnXor();
	int32_t ExtractFile();
};
