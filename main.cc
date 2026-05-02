#include "reader.h"

int main(int argc, char **argv) {
	FILE *input_file;
	if (argc != 2) return -1;
	if (!(input_file = std::fopen(argv[1], "r")))
		return -1;
	try {
		reader::Reader r(input_file);
		auto rules = r.ReadRules();
		for (auto &r : rules)
			std::puts(r.ToString().c_str());
	} catch (const reader::ReaderError &e) {
		std::puts(e.what());
	}
	std::fclose(input_file);
	return 0;
}
