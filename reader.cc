#include "reader.h"

namespace reader {

Reader::Reader(FILE *input) : input_(input) {}

bool Reader::ConsumeLine() {
	if (input_ == NULL) return false;
	char line[kMaxLengthOfLine];
	if (std::fgets(line, sizeof(line), input_) == NULL) return false;
	std::printf("%s", line);
	return true;
}

} // namespace reader
