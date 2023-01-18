#ifndef PRINTER_MODULE_HPP
#define PRINTER_MODULE_HPP

#include "IModule.hpp"
#include "EventChannel.hpp"
#include <fstream>
#include <iostream>

class PrinterModule : public IModule {
public:
	PrinterModule() {
		s = Subscriber::create("PrinterSubscriber");
		initEvents();
	}

	static IModule::NamedConstructor getNamedConstructor() {
		return {"PrinterModule", []()->IModule*{return new PrinterModule();}};
	}

	~PrinterModule() { }
	
	void initEvents() {
		EventChannel::getInstance().subscribe("NewData", s);
		s->addActionToTopic("NewData", "Print", [] (std::any msg) {
			auto data = std::any_cast<std::vector<uint8_t>>(msg);
			std::string str = std::string(data.begin(), data.end());
			std::cout << "New data: " << str << std::endl;
		});

		s->addActionToTopic("NewData", "PrintBlockingToFile", [](Subscriber::ActionArgument msg){
			auto data = std::any_cast<std::vector<uint8_t>>(msg);
			std::string str = std::string(data.begin(), data.end());
			std::ofstream outfile("log", std::ios_base::app);
			outfile << str << std::endl;
			sleep(1);
		});
	}

	Subscriber::Ptr s;
};

#endif