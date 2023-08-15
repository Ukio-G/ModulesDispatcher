#ifndef IMODULE_HPP
#define IMODULE_HPP

#include "EventChannel.hpp"
#include "ModuleInfo.hpp"
#include <string>

class ModuleDispatcher;

// Managed only by intermal module logic - you are responsible for the use of your EModuleStatus!
enum EModuleStatus {
	EMS_UNDEFINED = -1,

	EMS_LOADED = 0,
	EMD_CONFIGURED = 1,
	EMS_INITED = 2,
	EMS_STARTED = 3,
	EMS_PAUSED = 4,
	EMS_STOPPED = 5,

	EMS_MAX = 6,
};

class IModule
{
	friend ModuleDispatcher;
public:
	IModule() = delete;
	IModule(const std::string & name) : 
		name(name), 
		subscriber(Subscriber::create(name)) 
		{}

	virtual ~IModule() { };

	// Mandatory interface
	virtual void init() = 0;
	virtual void initEvents() = 0;

	// Optional interface
	virtual void start() {};
	virtual void pause() {};
	virtual void stop() {};
	virtual void update() {};

	// Interface with pre-defined behaviour
	const std::string & getName() { 
		return name; 
	};
	
	EModuleStatus getStatus() {
		return status;
	}

protected:
	void setOwner(ModuleDispatcher *owner) { 
		this->owner = owner; 
	}

	// If module use custom topic name for handle configuration it's possible to
	// override this function in derived class
	virtual std::string getConfigurationEventName() {
		return name + "Config";
	}

	ModuleDispatcher * owner;
	std::string name;
	EModuleStatus status = EMS_UNDEFINED;

	// Used for one-time initialization
	bool inited = false;
	bool eventsInited = false;
	bool configured = false;

	// Interaction with other modules and configuration receiving
	Subscriber::Ptr subscriber;
};

#endif