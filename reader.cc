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
	if (!ConsumeLine())
		throw ReaderError("There has to be a header.", line_number_);
	for (;;) {
		if (std::feof(input_))
			break;
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
		if (!ConsumeLine())
			throw ReaderError("The file is interrupted at a point where dependencies should be written.", line_number_);
		if (!IsIndentedLine()) {
			throw ReaderError("Indent lines to list dependencies when you write them.", line_number_);
		}
		auto contents = ReadDependencies();
		rules.emplace_back(std::move(comannds), std::move(contents));
	}
	return rules;
}

bool Reader::ConsumeLine() {
	if (!input_)
		return false;
	if (!std::fgets(line_, sizeof(line_), input_))
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

std::list<std::string> Reader::SplitString(const char *target, char by) {
	std::list<std::string> out;
	const char *begin = target;
	const char *end = target;
	for (;;) {
		if (*end == by || !*end) {
			out.emplace_back(begin, end - begin);
			if (!end)
				break;
			begin = ++end;
			continue;
		}
		++end;
	}
	return out;
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

#define SKIP_SPACE \
	while (std::isspace(*begin)) \
		++begin;\
	end = begin
#define MOVE_END_POINTER_TO_END_OF_WORD \
	while (*end && *end != '>' && !std::isspace(*end)) \
		++end
#define FIRST_TIME (!i)
#define SECOND_TIME (i)

std::list<node::Dependency> Reader::ReadDependencies() {
	std::list<node::Dependency> dependncies;
	while (IsIndentedLine()) {
		std::list<std::string> what_depend, what_they_depend_on;
		auto *ref_list = &what_depend;
		const char *begin, *end = line_;
		for (int i = 0; i != 2; ++i) {
			begin = end;
			for (;;) {
				if (std::isspace(*begin)) {
					SKIP_SPACE;
				}
				if (!*end) {
					if SECOND_TIME
						goto end;
					else
						throw ReaderError("Dependencies must have '>'.", line_number_);
				}
				if (*end == '>') {
					if FIRST_TIME
						break;
					else
						throw ReaderError("Dependencies must have '>'.", line_number_);
				}
				MOVE_END_POINTER_TO_END_OF_WORD;
				ref_list->emplace_back(begin, end - begin);
				begin = end;
			}
			ref_list = &what_they_depend_on;
			++end;
		}
	end:
		dependncies.emplace_back(std::move(what_depend), std::move(what_they_depend_on));
		if (!ConsumeLine())
			break;
	}
	return dependncies;
}

#undef MOVE_END_POINTER_TO_END_OF_WORD
#undef READ_WORD
#undef FIRST_TIME
#undef SECOND_TIME

} // namespace reader
