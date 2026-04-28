#ifndef MKAGE_NODE_H
#define MKAGE_NODE_H

#include <list>
#include <string>

namespace node {

using file_names = std::list<std::string>;

struct Dependency {
	file_names what_depend;
	file_names what_they_depend_on;

	std::string ToString();
};

struct Rule {
	Rule(std::list<std::string> &&cmds, std::list<Dependency> &&c);
	
	std::list<std::string> commands;
	std::list<Dependency> contents;

	std::string ToString();
};

} // namespace node

#endif
