#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "./cthulhu source_code log_file\n";
		return 1;
	}

	fs::path source_path(argv[1]);
	fs::path log_path(argv[2]);

	if (!fs::exists(source_path)) {
		std::cerr << "Source path does not exists\n";
		return 1;
	}
}
