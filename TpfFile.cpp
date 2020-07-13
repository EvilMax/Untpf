#include "TpfFile.h"
#include <fstream>
#include <iostream>
#include "mz.h"
#include "mz_os.h"
#include "mz_strm.h"
#include "mz_strm_pkcrypt.h"
#include "mz_strm_mem.h"
#include "mz_zip.h"
#include "mz_zip_rw.h"

TpfFile::TpfFile()
{
	data = nullptr;
	dataLen = 0;
	bufferLen = 0;
	loaded = false;
	xored = false;
}

TpfFile::TpfFile(string destination) : TpfFile() {
	this->destination = destination;
}

TpfFile::~TpfFile() {
	if (data != nullptr) {
		delete[] data;
	}
}

void TpfFile::SetFile(string name) {
	this->name = name;
	this->loaded = false;
	this->xored = false;
}

void TpfFile::ReadFile() {
	if (loaded)
	{
		return;
	}
	xored = false;

	ifstream inFile;
	try {
		inFile.open(name.c_str(), std::ifstream::binary);
		if (!inFile) {
			throw ios_base::failure("File open");
		}
	}
	catch (exception e) {
		cout << "Error opening file " << name << endl;
		throw e;
	}

	inFile.seekg(0, ios::end);
	dataLen = static_cast<uint32_t> (inFile.tellg());
	if (dataLen == 0) {
		cout << "File is zero length!" << endl;
		throw "File length";
	}
	inFile.seekg(0, ios::beg);

	if (dataLen >= bufferLen) {
		if (data != nullptr) {
			delete[] data;
			data = nullptr;
		}
		try {
			data = new uint8_t[(size_t)dataLen + 1];
		}
		catch (exception e) {
			data = nullptr;
			dataLen = 0;
			bufferLen = 0;
			cout << "Error allocating " << dataLen + 1 << " bytes of memory" << endl;
			throw e;
		}
		bufferLen = dataLen + 1;
	}

	inFile.read((char*)data, dataLen);
	long result = static_cast<unsigned int> (inFile.gcount());
	inFile.close();

	if (result != dataLen) {
		cout << "Error reading file " << name << " : read " << result << " bytes instead of " << dataLen << endl;
		dataLen = 0;
		throw "File read error";
	}

	data[dataLen] = 0;
	loaded = true;
}

void TpfFile::UnXor() {
	if (xored) {
		return;
	}
	uint32_t* buff = (uint32_t*)data;
	uint32_t TPF_XOR_MAGIC = 0x3FA43FA4u;
	uint32_t size = dataLen / 4u;
	for (unsigned int i = 0; i < size; i++) {
		buff[i] ^= TPF_XOR_MAGIC;
	}
	for (uint32_t i = size * 4u; i < size * 4u + dataLen % 4u; i++) {
		((uint8_t*)data)[i] ^= (uint8_t)TPF_XOR_MAGIC;
	}

	uint32_t pos = dataLen - 1;
	while (pos > 0u && data[pos]) {
		pos--;
	}
	if (pos > 0u && pos < dataLen - 1) {
		dataLen = pos + 1;
	}
	xored = true;
}

int32_t minizip_extract_entry_cb(void* handle, void* userdata, mz_zip_file* file_info, const char* path) {
	MZ_UNUSED(handle);
	MZ_UNUSED(userdata);
	MZ_UNUSED(path);
	printf("Extracting %s\n", file_info->filename);
	return MZ_OK;
}

int32_t TpfFile::ExtractFile() {
	void* reader = NULL;
	int32_t err = MZ_OK;
	int32_t err_close = MZ_OK;
	const char* pattern = "*";

	mz_zip_reader_create(&reader);
	mz_zip_reader_set_pattern(reader, pattern, 1);
	mz_zip_reader_set_password(reader, pw);
	mz_zip_reader_set_entry_cb(reader, NULL, minizip_extract_entry_cb);

	err = mz_zip_reader_open_buffer(reader, data, dataLen, 0);

	if (err != MZ_OK) {
		printf("Error %" PRId32 " opening TPF file.\n", err);
	}
	else {
		err = mz_zip_reader_save_all(reader, destination.c_str());

		if (err == MZ_END_OF_LIST) {
			if (pattern != NULL) {
				printf("Files matching %s not found in archive\n", pattern);
			}
			else {
				printf("No files in archive\n");
				err = MZ_OK;
			}
		}
		else if (err != MZ_OK) {
			printf("Error %" PRId32 " saving entries to disk\n", err);
		}
	}

	err_close = mz_zip_reader_close(reader);
	if (err_close != MZ_OK) {
		printf("Error %" PRId32 " closing archive for reading\n", err_close);
		err = err_close;
	}

	mz_zip_reader_delete(&reader);
	return err;
}

void TpfFile::Unpack() {
	ReadFile();
	UnXor();
	ExtractFile();
}