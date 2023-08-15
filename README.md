## Modules dispatcher

### Synopsis 
Lightweight header-only construction, allows you to wrap you components and re-organize application to modules composition.

EventChannel and EventLoop provides event driven design for modules communication.

Supported loading modules from shared libraries (tested with GCC compiler)

### How to use
- Create module - wrapper for you component. It's just a class inherited from `IModule` class.
- Include ModuleDispatcher.hpp, create ModuleDispatcher object and instance module: 
```c++
ModuleDispatcher md;

// Load configurations about modules (see config.json for details)
md.loadConfigurations("./config.json");

// Register module from ModuleInfo (used compile-time registration)
md.registerModuleInfo(TimerModule::ModuleInfo());

// Load and register module from file (runtime registrations)
md.loadModuleFromShared("./libTestModule.so");
```
- Implement some events in you `Module` for communicating with other modules.
- After all, you are able to destroy you module:
```c++
md.destroyModule(id);
```
Also, in `example/main.cpp` you can find simple code-example.


### Configuration example

Example of config.json:
```
{
    "modulesConfiguration" : {
        "TestModuleConfig" : {
            "i" : 1,
            "j" : 2,
            "k" : 3,
            "m" : 10
        },

        "TimerModuleConfig" : {
            "tickInterval" : 100
        }
    },

    "modulesToStart" : [
        "TestModule",
        "TimerModule"
    ]
}
```
- modulesToStart: contains module's names to start
- modulesConfiguration: contains pair str:  {json}, where str - event name for configuration handle and {json} - configuration itself. 
    
For example 
```
"TestModuleConfig" : {
        "i" : 1,
        "j" : 2,
        "k" : 3,
        "m" : 10
},
```

TestModuleConfig - event name, and corresponding part - configuration for module "TestModule"
### How to use in my project?

Run bootstrap.sh after git clone:
```
cd <root you project dir>
git clone https://github.com/Ukio-G/ModulesDispatcher.git
cd ModulesDispatcher
./bootstrap.sh
```

or make script's actions manualy (However, the project will still need to be cloned)
```
cd <root you project dir>
git clone https://github.com/Ukio-G/ModulesDispatcher.git
cd ModulesDispatcher

mkdir -p include/modules
mv include/* include/modules
mv include ..

# Remove examples and other unusable stuff

REPO_DIR=`pwd`
cd ..
rm -r $REPO_DIR
```

### Dependencies

- https://github.com/nlohmann/json - for parsing configuration's files

### ToDo
- (Done!) Loading modules from json file
- (Done by ModuleInfo) Append string pseudonym for modules. Something like `using NamedModuleConstuctor = std::pair<std::string, std::function<IModule*()>` returned from static module function. `ModuleDispatcher` shall collect `NamedModuleConstuctor` instances and create new object, inherited from `IModule` based on passed `std::string` key:
```c++
// Register module from ModuleInfo (used compile-time registration)
md.registerModuleInfo(TimerModule::ModuleInfo());
md.instanceModule("TimerModule"); // TimerModule::ModuleInfo().name == "TimerModule"
```