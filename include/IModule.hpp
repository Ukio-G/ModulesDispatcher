#ifndef IMODULE_HPP
#define IMODULE_HPP

#include <typeindex>
#include <functional>
#include "ModuleInfo.hpp"

class ModuleDispatcher;

class IModule
{
	friend ModuleDispatcher;
public:
	using NamedConstructor = std::pair<std::string, std::function<IModule*()>>;
	IModule() { }
	virtual ~IModule() { };

	NamedConstructor getType();
private:
	void setOwner(ModuleDispatcher *owner) { this->owner = owner; }
	ModuleDispatcher * owner;
};

#endif