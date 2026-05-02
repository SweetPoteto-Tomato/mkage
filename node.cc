#include "node.h"

#include <utility>

namespace node {

Dependency::Dependency(file_names &&wd, file_names &&wtdo)
	: what_depend(std::move(wd)), what_they_depend_on(std::move(wtdo)) {}

std::string Dependency::ToString() {
	std::string out = "Dependency (";
	for (auto &wd : what_depend)
		out += wd + ' ';
	out += ',';
	for (auto &wtdo : what_they_depend_on)
		out += wtdo + ' ';
	out += ')';
	return out;
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
