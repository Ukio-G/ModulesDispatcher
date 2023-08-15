#ifndef MODULE_DISPATCHER_HPP
#define MODULE_DISPATCHER_HPP

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>
#include <utility>

#include "EventChannel.hpp"
#include "IModule.hpp"
#include "ModuleInfo.hpp"

#include <dlfcn.h>

#include "json.hpp"


class ModuleDispatcher
{
public:
	ModuleDispatcher() { }
	~ModuleDispatcher() { }

	// Make sure modules loaded this way have been compiled with the -rdynamic flag (GCC)
	ModuleInfo loadModuleFromShared(const std::string & filename) {
		auto lib = dlopen(filename.c_str(), RTLD_NOW | RTLD_GLOBAL);

		if (!lib)
			throw std::runtime_error("Unable to open: " + filename);

		// Shared library, which contain module, should also contain function "ModuleInfo* init()", which allocate 
		// ModuleInfo with all neccecery information for the module
        auto get_module_info = reinterpret_cast<ModuleInfo(*)()>(dlsym(lib, "getModuleInfo"));

		if (!get_module_info)
			throw std::runtime_error("Unable to find getModuleInfo function in " + filename);

		ModuleInfo moduleInfo = get_module_info();
		registerModuleInfo(moduleInfo);
		return moduleInfo;
	}

	int instanceModule(const std::string & name) {
		if (registeredModules.find(name) == registeredModules.end())
			return -1;

		auto moduleInfo = registeredModules[name];
		IModule* modulePtr = moduleInfo.constructior();

		auto newModuleId = getFreeId();
		modulePtr->owner = this;

		std::cout << "module " << name << " instances with id = " << newModuleId << std::endl;

		modules[newModuleId] = modulePtr;
		return newModuleId;
	}

	template <typename ModuleT, typename ... ModuleArgs>
	int instanceModule(ModuleArgs && ... args) {
		ModuleT* modulePtr = new ModuleT(std::forward<ModuleArgs> (args) ...);

		auto newModuleId = getFreeId();
		modulePtr->owner = this;

		modules[newModuleId] = modulePtr;
		return newModuleId;
	}

	unsigned int getFreeId() {
		return idCounter++;
	}

	IModule* getModuleById(unsigned int id) {
		auto it = modules.find(id);
		if (it == modules.end())
			return nullptr;
		
		auto modulePtr = it->second;

		if (!modulePtr) {
			std::cout << "Module with id " << id << " is not active." << std::endl;
			return nullptr;
		}

		return modulePtr;
	}

	void destroyModule(unsigned int id) {
		auto moduleIt = modules.find(id);
		if (moduleIt == modules.end()) { 
			std::cout << "Module with id " << id << " is not active." << std::endl;
			return; 
		}

		delete moduleIt->second;
		modules.erase(moduleIt);
	}

    void registerModuleInfo(const ModuleInfo & moduleInfo) {
		registeredModules[moduleInfo.moduleName] = moduleInfo;
    }

	void loadConfigurations(const std::string & filename) {
		nlohmann::json j;
		std::ifstream ifs(filename);
		if (ifs.fail()){
			throw std::runtime_error("Configuration file " + filename + " not found");
    	}

		j = nlohmann::json::parse(ifs);

		modulesConfiguration = j["modulesConfiguration"];
		modulesToStart = j["modulesToStart"];
	}

	void instanceModulesFromConfig() {
		for(auto &moduleName: modulesToStart) {
			instanceModule(moduleName);
		}
	}

	void initModulesEvents() {
		for (auto &[id, module] : modules) {
			module->initEvents();
		}
	}

	void pushConfigToModules() {
		for (auto & config : modulesConfiguration.items()) {
			EventChannel::getInstance().publish(config.key(), config.value());
		}
	}

	void initModules() {
		for (auto &[id, module] : modules) {
			module->init();
		}
	}

	void startModules() {
		for (auto &[id, module] : modules) {
			module->start();
		}
	}
	void updateModules() {
		for (auto &[id, module] : modules) {
			module->update();
		}
	}

	void pauseModules() {
		for (auto &[id, module] : modules) {
			module->pause();
		}
	}

	void stopModules() {
		for (auto &[id, module] : modules) {
			module->stop();
		}
	}

	EModuleStatus getModuleStatus(int id) const {
		return modules.at(id)->getStatus();
	}

private:
	using ModulesT = std::map<int, IModule*>;
	using activeModulesIt = ModulesT::iterator;

	IModule* findFirstModuleByName(const std::string & name) {
		return nullptr;
	}

	ModulesT modules;
	std::unordered_map<std::string, ModuleInfo> registeredModules;

	int idCounter = 0;

	nlohmann::json modulesConfiguration;
	nlohmann::json modulesToStart;
};

#endif