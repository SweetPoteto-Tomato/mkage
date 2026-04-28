#ifndef MKAGE_READER_H
#define MKAGE_READER_H

#include <cstdio>

#include "node.h"

namespace reader {

constexpr int kMaxLengthOfLine = 80;

class ReaderError : public std::runtime_error {
public:
	ReaderError(const char *message, int line);

	const char *what() const;

private:
	char message_[80];
};

class Reader {
public:
	Reader(FILE *input);

	std::list<node::Rule> ReadRules();

private:
	FILE *input_;
	int line_number_;
	char line_[kMaxLengthOfLine];

	bool ConsumeLine();
	bool IsIndentedLine();
	std::list<std::string> ReadCommands();
};

} // namespace reader

#endif
