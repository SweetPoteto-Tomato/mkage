#include "reader.h"

#include <cctype>
#include <cstring>
#include <utility>

namespace reader {

ReaderError::ReaderError() : std::runtime_error("ReaderError") {}

const char *ReaderError::what() const {
	return "ReaderError";
}

Reader::Reader(FILE *input) : input_(input) {}

std::list<node::Rule> Reader::ReadRules() {
	std::list<node::Rule> rules;
	for (;;) {
		ConsumeLine();
		if (!IsIndentedLine()) {
			throw ReaderError();
		}
		ConsumeLine();
		if (IsIndentedLine()) {
			throw ReaderError();
		}
		auto comannds = ReadCommands();
		if (std::strcmp("in", line_))
			throw ReaderError();
		std::list<node::Dependency> contents;
		rules.emplace_back(std::move(comannds), std::move(contents));
	}
	return rules;
}

void Reader::ConsumeLine() {
	if (input_ == NULL)
		throw ReaderError();
	if (std::fgets(line_, sizeof(line_), input_) == NULL)
		throw ReaderError();
	char *c;
	for (c = line_; *c && !std::isspace(*c); ++c) {}
	*c = '\0';
}

bool Reader::IsIndentedLine() {
	return std::isspace(line_[0]);
}

std::list<std::string> Reader::ReadCommands() {
	std::list<std::string> commands;
	ConsumeLine();
	while (IsIndentedLine()) {
		commands.emplace_back(line_);
		ConsumeLine();
	}
	return commands;
}

} // namespace reader
