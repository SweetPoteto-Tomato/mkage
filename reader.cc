#include "reader.h"

#include <cctype>
#include <cstring>
#include <utility>

namespace reader {

ReaderError::ReaderError(const char *message, int line)
	: std::runtime_error("ReaderError") {
	std::snprintf(message_, sizeof(message_), "(%d)%s", line, message);
}

const char *ReaderError::what() const {
	return message_;
}

Reader::Reader(FILE *input) : input_(input), line_number_(0) {}

std::list<node::Rule> Reader::ReadRules() {
	std::list<node::Rule> rules;
	for (;;) {
		if (!ConsumeLine()) {
			if (std::feof(input_))
				break;
			else
				throw ReaderError("There has to be a header.", line_number_);
		}
		if (IsIndentedLine()) {
			throw ReaderError("Headers should be not indented.", line_number_);
		}
		if (!ConsumeLine())
			throw ReaderError("The file is interrupted at a point where rules should be written.", line_number_);
		if (!IsIndentedLine()) {
			throw ReaderError("Indent lines to list rules when you write them.", line_number_);
		}
		auto comannds = ReadCommands();
		if (std::strcmp("in", line_))
			throw ReaderError("There has to be \"in\".", line_number_);
		std::list<node::Dependency> contents;
		rules.emplace_back(std::move(comannds), std::move(contents));
	}
	return rules;
}

bool Reader::ConsumeLine() {
	if (input_ == NULL)
		return false;
	if (std::fgets(line_, sizeof(line_), input_) == NULL)
		return false;
	char *c;
	for (c = line_; *c && *c != '\n' && *c != '\r'; ++c) {}
	*c = '\0';
	++line_number_;
	return true;
}

bool Reader::IsIndentedLine() {
	return line_[0] == '\t';
}

std::list<std::string> Reader::ReadCommands() {
	std::list<std::string> commands;
	while (IsIndentedLine()) {
		commands.emplace_back(line_);
		if (!ConsumeLine())
			throw ReaderError("Reader reached EOF while reading the file.", line_number_);
	}
	return commands;
}

} // namespace reader
