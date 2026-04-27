#ifndef MKAGE_READER_H
#define MKAGE_READER_H

#include <cstdio>

namespace reader {

constexpr int kMaxLengthOfLine = 80;

class Reader {
public:
	Reader(FILE *input);

	bool ConsumeLine();

private:
	FILE *input_;
};

} // namespace reader

#endif
