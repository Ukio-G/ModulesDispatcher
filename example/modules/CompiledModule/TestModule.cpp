#include "TestModule.hpp"
#include "ModuleInfo.hpp"
#include "json.hpp"
#include <chrono>
#include <ratio>
#include <thread>

void TestModule::init()  {
    if (inited)
        return;
    std::cout << "[TestModule] inited" << std::endl;
};

void TestModule::initEvents() {
    if (eventsInited)
        return;

    // Maybe move it to base IModule class? To avoid dublication in all modules
    EventChannel::getInstance().subscribe(getConfigurationEventName(), subscriber); 

    subscriber->addActionToTopic(getConfigurationEventName(), "UpdateM", [this](std::any data){
        sleep(2); // simulating a configuration receive delay
        config = std::any_cast<nlohmann::json>(data);
        inited = true;
    });

    eventsInited = true;
}

void TestModule::configure() {
    waitConfigurationCondition([this](){
        return !(config.empty());
    });
    m = config["m"];
    configured = true;
}

void TestModule::start() {
    waitConfigurationDone();
    std::cout << "[TestModule] started with M value = " << m << std::endl;
}

extern "C" {
    ModuleInfo getModuleInfo() {
        TestModule::ModuleInfo moduleInfo;
        return moduleInfo;
    }
};