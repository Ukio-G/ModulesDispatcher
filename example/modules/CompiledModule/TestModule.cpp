#include "TestModule.hpp"
#include "ModuleInfo.hpp"
#include "json.hpp"
#include <chrono>
#include <ratio>
#include <thread>

void TestModule::init()  {
    if (inited)
        return;
    inited = true;
    std::cout << "TestModule inited" << std::endl;
};

void TestModule::initEvents() {
    if (eventsInited)
        return;

    // Maybe move it to base IModule class? To avoid dublication in all modules
    EventChannel::getInstance().subscribe(getConfigurationEventName(), subscriber); 

    subscriber->addActionToTopic(getConfigurationEventName(), "UpdateM", [this](std::any data){
        auto config = std::any_cast<nlohmann::json>(data);
        m = config["m"];
        configured = true;
    });
    eventsInited = true;
}

void TestModule::start() {
    while (!configured) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << "[TestModule] start" << std::endl;
    std::cout << "[TestModule] M value = " << m << std::endl;
}

extern "C" {
    ModuleInfo getModuleInfo() {
        TestModule::ModuleInfo moduleInfo;
        return moduleInfo;
    }
};