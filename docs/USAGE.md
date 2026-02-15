## Usage

### 1) Define a module

```cpp
#include <ModuleDispatcher/IModule.hpp>
#include <ModuleDispatcher/ModuleInfo.hpp>

class TimerModule : public IModule {
public:
    TimerModule() : IModule("TimerModule") {}

    static ModuleInfo ModuleInfo() {
        return {"TimerModule", []() -> IModule* { return new TimerModule(); }};
    }

    void initEvents() override {
        subscriber->addActionToTopic(getConfigurationEventName(), [this](Subscriber::ActionArgument data) {
            config = std::any_cast<nlohmann::json>(data);
        });
        EventChannel::getInstance().subscribe(getConfigurationEventName(), subscriber);
    }

    void init() override {}
    void start() override {}
};
```

### 2) Configure and run dispatcher

```cpp
#include <ModuleDispatcher/ModuleDispatcher.hpp>

int main() {
    ModuleDispatcher md;

    md.loadConfigurations("./config.json");
    md.registerModuleInfo(TimerModule::ModuleInfo());
    md.instanceModulesFromConfig();

    md.initModulesEvents();
    md.pushConfigToModules();
    md.initModules();
    md.configureModules();
    md.startModules();

    return 0;
}
```

### 3) Runtime plugin loading

`ModuleDispatcher` can load module metadata from a shared object:

```cpp
md.loadModuleFromShared("./libMyModule.so");
```

Shared library must export:

```cpp
extern "C" ModuleInfo getModuleInfo();
```

## Config format

```json
{
  "modulesConfiguration": {
    "TimerModuleConfig": {
      "tickInterval": 100
    }
  },
  "modulesToStart": [
    "TimerModule"
  ]
}
```
