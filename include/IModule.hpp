#ifndef IMODULE_HPP
#define IMODULE_HPP

#include "EventChannel.hpp"
#include "ModuleInfo.hpp"
#include "json.hpp"
#include <chrono>
#include <string>
#include <thread>
#include <future>

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
	virtual void configure() {};
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

	void waitStatus(EModuleStatus waitStatus) {
		while (status != waitStatus) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	void waitInitializeDone() {
		while (!inited) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	void waitEventsInitializeDone() {
		while (!eventsInited) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	void waitConfigurationDone() {
		while (!configured) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	void waitConfigurationReceived() {
		while (config.empty()) {
			std::cout << "Wait config" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	void waitConfigurationCondition(std::function<bool()> f) {
		auto a3 = std::async(std::launch::async, [this, f]()->void {
			while(!f()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
			configured = true;
		});
		a3.wait();
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
	nlohmann::json config;
	EModuleStatus status = EMS_UNDEFINED;

	// Used for one-time initialization
	bool inited = false;
	bool eventsInited = false;
	bool configured = false;

	// Interaction with other modules and configuration receiving
	Subscriber::Ptr subscriber;
};

#endif