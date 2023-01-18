#ifndef MODULE_DISPATCHER_HPP
#define MODULE_DISPATCHER_HPP

#include <fstream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>
#include <utility>

#include "IModule.hpp"
#include "ModuleInfo.hpp"

class ModuleDispatcher
{
public:
	ModuleDispatcher() { }
	~ModuleDispatcher() { }

	template <typename ModuleT, typename ... ModuleArgs>
	unsigned int instanceModule(const std::string & name, ModuleArgs && ... args) {
		IModule* modulePtr = new ModuleT(std::forward<ModuleArgs> (args) ...);

		auto newModuleId = getFreeId();
		modulePtr->owner = this;

		ModuleInfo info = {newModuleId, name};
		activeModules.insert(std::pair(info, modulePtr));
		return newModuleId;
	}

	unsigned int getFreeId() {
		return idCounter++;
	}

	template <typename ModuleT>
	ModuleT* getModuleById(unsigned int id) {
		IModule* modulePtr = findModuleById(id);
		
		if (!modulePtr) {
			std::cout << "Module with id " << id << " is not active." << std::endl;
			return nullptr;
		}

		if (!(modulePtr = dynamic_cast<ModuleT*>(modulePtr))) {
			std::cout << "Requested and passed module types not match." << std::endl;
			return nullptr;
		}

		return modulePtr;
	}

	void destroyModule(unsigned int id) {
		auto moduleIt = findIteratorById(id);
		if (moduleIt == activeModules.end()) { 
			std::cout << "Module with id " << id << " is not active." << std::endl;
			return; 
		}

		delete moduleIt->second;
		activeModules.erase(moduleIt);
	}

private:
	using ActiveModulesT = std::map<ModuleInfo, IModule*>;
	using activeModulesIt = ActiveModulesT::iterator;

	activeModulesIt findIteratorById(unsigned int id) {
		auto moduleIt = std::find_if(activeModules.begin(), activeModules.end(), 
			[id](const auto& info) { return info.first.id == id; });
		return moduleIt;
	}

	IModule* findModuleById(unsigned int id) {
		auto moduleIt = findIteratorById(id);
		return (moduleIt == activeModules.end()) ? nullptr : moduleIt->second; 
	}

	IModule* findFirstModuleByName(const std::string & name) {
		auto moduleIt = std::find_if(activeModules.begin(), activeModules.end(), 
			[&name](const auto& info) { return info.first.moduleName == name; });
		if (moduleIt == activeModules.end())
			return nullptr;
		return moduleIt->second;
	}	

	ActiveModulesT activeModules;
	unsigned int idCounter;
};

#endif