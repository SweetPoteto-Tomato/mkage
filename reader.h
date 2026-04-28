#ifndef MKAGE_READER_H
#define MKAGE_READER_H

#include <cstdio>

#include "node.h"

namespace reader {

constexpr int kMaxLengthOfLine = 80;

class ReaderError : public std::runtime_error {
public:
	ReaderError();

	const char *what() const;
};

class Reader {
public:
	Reader(FILE *input);

	std::list<node::Rule> ReadRules();

private:
	FILE *input_;
	char line_[kMaxLengthOfLine];

	void ConsumeLine();
	bool IsIndentedLine();
	std::list<std::string> ReadCommands();
};

} // namespace reader

#endif
