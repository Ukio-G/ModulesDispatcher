## ModulesDispatcher

Minimal include-only C++ library for event-driven module orchestration.

## Include Style

```cpp
#include <ModuleDispatcher/ModuleDispatcher.hpp>
#include <ModuleDispatcher/IModule.hpp>
#include <ModuleDispatcher/ModuleInfo.hpp>
```

## Quick Integration (Recommended)

1. Add this repository into your project, for example:

```text
MyApp/
  CMakeLists.txt
  src/main.cpp
  external/ModulesDispatcher/
```

2. In your project `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyApp LANGUAGES CXX)

add_executable(my_app src/main.cpp)
target_compile_features(my_app PRIVATE cxx_std_17)

add_subdirectory(external/ModulesDispatcher)

target_include_directories(my_app PRIVATE
  ${ModulesDispatcher_INCLUDE_DIR}
)
```

3. In your `src/main.cpp`:

```cpp
#include <ModuleDispatcher/ModuleDispatcher.hpp>
#include <ModuleDispatcher/IModule.hpp>
#include <ModuleDispatcher/ModuleInfo.hpp>

int main() {
    ModuleDispatcher md;
    return 0;
}
```

4. Build:

```bash
cmake -S . -B build
cmake --build build
```

## Update

If vendored inside your project:

```bash
cd external/ModulesDispatcher
git pull
```
