#ifndef MODULE_INFO_HPP
#define MODULE_INFO_HPP

#include <string>

class IModule;

struct ModuleInfo {
    typedef IModule*(*ConstructorPtr)();
    
    ModuleInfo() {}

    ModuleInfo(const ModuleInfo& other) : 
        moduleName(other.moduleName), 
        constructior(other.constructior) 
        { }

    ModuleInfo(const std::string & name, ConstructorPtr constructorPtr) : 
        moduleName(name), 
        constructior(constructorPtr) 
        { }

    std::string moduleName;
	ConstructorPtr constructior;
};


#endif