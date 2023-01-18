#ifndef MODULE_APP_TIMERMODULE_HPP
#define MODULE_APP_TIMERMODULE_HPP

#include <iostream>
#include "IModule.hpp"
#include "EventChannel.hpp"

class TimerModule: public IModule {
public:
	TimerModule() {
		myLoop.addFunction<ECP_ENDLESS>([this]() {tick();});
	}

	~TimerModule() { }

	void tick() {
		std::cout << "tick" << std::endl;
	}
private:
	 EventLoop myLoop;
};

#endif //MODULE_APP_TIMERMODULE_HPP
