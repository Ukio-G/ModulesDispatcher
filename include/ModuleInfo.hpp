#ifndef MODULE_INFO_HPP
#define MODULE_INFO_HPP

#include <string>

struct ModuleInfo {
	uint id;
	std::string moduleName;

	bool operator<(const ModuleInfo & other) const { return id < other.id;}
	bool operator<(ModuleInfo & other) { return id < other.id;}
};

#endif