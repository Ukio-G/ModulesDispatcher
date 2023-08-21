#ifndef __TESTMODULE__H__
#define __TESTMODULE__H__

#include "IModule.hpp"
#include "ModuleInfo.hpp"

class TestModuleInfo;

class TestModule : public IModule {
    public:
    using ModuleInfo = TestModuleInfo;
    TestModule() : IModule("TestModule") { initEvents(); }
	void init();
	void initEvents();
    void start();
    void configure();
    private:
    int m = 0;
};


class TestModuleInfo : public ModuleInfo {
    public:
    TestModuleInfo() : ModuleInfo("TestModule", []() -> IModule* {return new TestModule();}) { }
};


#endif  //!__TESTMODULE__H__