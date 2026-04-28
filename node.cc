#include "node.h"

#include <utility>

namespace node {

std::string Dependency::ToString() {
	return std::string();
}

Rule::Rule(std::list<std::string> &&cmds, std::list<Dependency> &&c)
	: commands(std::move(cmds)), contents(std::move(c)) {}

std::string Rule::ToString() {
	std::string out = "Rule (";
	for (auto &cmd : commands)
		out += cmd + ' ';
	out += ',';
	for (auto &c : contents)
		out += c.ToString() + ' ';
	out += ')';
	return out;
}

} // namespace node
