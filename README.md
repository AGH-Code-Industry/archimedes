# Archimedes
Archimedes - Game Engine

# Installation
Archimedes is actually a C++ library. A tested way to link it into your game
is using `git submodule`. 

First, clone this repo into your directory with dependencies using `git submodule` command.

``` sh
git submodule add 'https://github.com/AGH-Code-Industry/archimedes' <dependencies>/archimedes
```

Then, you can include it in your CMake project.

``` cmake
...

add_subdirectory(extern/archimedes)
target_link_libraries(${PROJECT_NAME} archimedes)

...
```

# Updating to newest version
If you have followed [Installation](#Installation) section, you should be able
to bump archimedes version using 
``` sh
git submodule update --remote
```

# Compiling
```sh
pip install conan
conan profile detect --force
mkdir build
conan install . --output-folder=build --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```
# Running Tests
If you have followed [Compiling](#Compiling) section, you should be able run tests
```sh
cd build
./test_archimedes
```

# Project structure
```
- archimedes
  - CMakeLists.txt
  - README.md
  - LICENSE.md
  - extern
      - ... <zależności, które postanowimy trzymać razem z projektem np. glad>
  - include
      - ... <headery .h projektu>
  - src
      - ... <pliki źródłowe .cpp projektu> 
  - tests
      - ... <pliki źródłowe testów jednostkowych> 
```
