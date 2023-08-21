#include "EventChannel.hpp"
#include "ModuleDispatcher.hpp"
#include "ModuleInfo.hpp"
#include "example/modules/TimerModule.hpp"
#include <unistd.h>

int main() {
	ModuleDispatcher md;

	// Load configurations about modules (see config.json for details)
	md.loadConfigurations("./config.json");

	// Register module from ModuleInfo (used compile-time registration)
	md.registerModuleInfo(TimerModule::ModuleInfo());

	// Load and register module from file (runtime registrations)
	md.loadModuleFromShared("./libTestModule.so");
	
	// Allocate memory for modules (instancing)
	md.instanceModulesFromConfig();

	// Initialization modules events
	// (including events that accept and process configurations, so do not neglect this stage)
	md.initModulesEvents();

	// Send readed from config.json configurations to EventChannel
	md.pushConfigToModules();

	// Init modules (run init() method on modules)
	md.initModules();

	md.configureModules();

	// Start modules (run start() method on modules)
	md.startModules();

	sleep(5);

	md.destroyModule(0);
	md.destroyModule(1);

	sleep(3);


	return 0;
}