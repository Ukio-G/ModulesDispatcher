## Modules dispatcher

### Synopsis 
Lightweight header-only construction, allows you to wrap you components and re-organize application to modules composition.

EventChannel and EventLoop provides event driven design for modules communication.

### How to use
- Create module wrappers for you components. It's just a class inherited from `IModule` class.
- Include ModuleDispatcher.hpp, create ModuleDispatcher object and instance module: 
```c++
ModuleDispatcher dispatcher;
unsigned int id = dispatcher.instanceModule<Module>("ModuleName");
```
- Implement some events in you `Module` for communicating with other modules.
- After all, you are able to destroy you module:
```c++
dispatcher.destroyModule(id);
```
Also, in `example/main.cpp` you can find simple code-example.

### ToDo
- Loading modules from json file
- Append string pseudonym for modules. Something like `using NamedModuleConstuctor = std::pair<std::string, std::function<IModule*()>` returned from static module function. `ModuleDispatcher` shall collect `NamedModuleConstuctor` instances and create new object, inherited from `IModule` based on passed `std::string` key:
```c++
ModuleDispatcher dispatcher;
dispatcher.addModuleConstructor(Module::getNamedConstructor()); // Module::getInfo() return something like {"Module", []() ->IModule* {new Module();} } 
dispatcher.instanceModule("Module"); // instance new Module object, using passed lambda
```