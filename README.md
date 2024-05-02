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

Prerequred:
- Conan 2.x
- git
- CMake >= 3.19

```sh
git clone https://github.com/AGH-Code-Industry/archimedes
```
```sh
cd archimedes
```

```sh
cmake . -B build -DCMAKE_BUILD_TYPE=<Release/Debug> -DARCHIMEDES_FORCE_CONAN_INSTALL=TRUE
```
```sh
cmake --build ./build
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
  - conanfile.py
  - CMakeLists.txt
  - README.md
  - LICENSE.md
  - cmake
      - ... <pliki cmake>
  - include
      - ... <pliki nagłówkowe .h i implementacje szablonów .hpp>
  - src
      - ... <pliki źródłowe .cpp>
  - archimedes_bin
      - ... <przykładowy projekt testowy>
  - tests
      - ... <pliki źródłowe .cpp testów jednostkowych>
```
