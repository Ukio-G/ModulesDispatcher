#ifndef MODULE_APP_TIMERMODULE_HPP
#define MODULE_APP_TIMERMODULE_HPP

#include <any>
#include <iostream>
#include "EventLoop.hpp"
#include "IModule.hpp"
#include "EventChannel.hpp"
#include "ModuleInfo.hpp"
#include "json.hpp"

class TimerModuleInfo;

class TimerModule: public IModule {
public:
	TimerModule() : IModule("TimerModule") { initEvents(); };
	using ModuleInfo = TimerModuleInfo;

	void initEvents() {
		if (eventsInited)
        	return;
		subscriber->addActionToTopic(getConfigurationEventName(), "UpdateTicks", [this](std::any data) {
			waitInitializeDone();
        	auto config = std::any_cast<nlohmann::json>(data);
			myLoop->setSleepTimeMS(config["tickInterval"]);
		});
		EventChannel::getInstance().subscribe(getConfigurationEventName(), subscriber);
		eventsInited = true;
	}

	void init() {
		if (inited)
        	return;
		myLoop = new EventLoop();
		inited = true;
	}

	void start() {
		waitConfigurationDone();
		myLoop->addFunction<ECP_ENDLESS>([this]() {tick();});
	}

	~TimerModule() {
		delete myLoop;
	 }

	void configure() {
		configured = true;
	}

	void tick() {
		static int i = 0;
		std::cout << "[TimerModule] tick #" << i++ << std::endl;
	}

private:
	EventLoop *myLoop = nullptr;
};

class TimerModuleInfo : public ModuleInfo {
	public:
	TimerModuleInfo() : ModuleInfo("TimerModule", []() -> IModule* {return new TimerModule();}) { }
};

#endif //MODULE_APP_TIMERMODULE_HPP
