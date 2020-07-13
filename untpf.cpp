#include <iostream>
#include <vector>
#include "TpfFile.h"

using namespace std;

void untpf_help() {
    printf("Usage: untpf -d dir <tpf files>\n\n" \
        "  -d  Destination directory\n\n");
	
}

int main(int argc, char** argv) {
	vector<char*> tpfFiles;
	const char* destination = NULL;
	if (argc == 1) {
		untpf_help();
		return 0;
	}
	for (int i = 1; i < argc; i += 1) {
		if (argv[i][0] == '-') {
			char c = argv[i][1];
			if (c == 'd' && (i + 1 < argc)) {
				destination = argv[i + 1];
				i++;
			}
		}
		else {
			tpfFiles.push_back(argv[i]);
		}
	}
	TpfFile *f = new TpfFile(destination);
	for each (char* var in tpfFiles)
	{
		f->SetFile(var);
		f->Unpack();
	}
	
	delete f;

	return 0;
}