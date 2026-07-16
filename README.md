# libtokenizer

Tokenizer library in Cpp23

> [!IMPORTANT]
> Since this library is still in development, it may not work correctly or some
> functionalities might be missing

## Build

### Requirements

- [CMake](https://cmake.org/)

### Compilation

```bash
git clone https://github.com/giorgi.page/libtokenizer.git
cd libtokenizer

./build.sh
```

### Testing

```bash
cmake -B build \
    -DTOKENIZER_BUILD_TESTING=ON \
    && cmake --build build --parallel $(($(nproc) - 1)) \
    && ctest --test-dir build 
```

### Example

```bash
cmake -B build \
    -DTOKENIZER_BUILD_EXAMPLE=ON \
    && cmake --build build --parallel $(($(nproc) - 1))

cd example
./build/example
```

## Usage

```cpp
/*
 * main.cxx
 */

#include <iostream>

#include "libparser/errors/file_error.hxx"
#include "libtokenizer/tokenizer.hxx"

using namespace libtokenizer;

auto main(void) -> int {
    static constexpr auto filepath {"example.txt"};

    try {
        tokenizer p {filepath};
        for (const auto& token : p.tokens())
            std::cout << "{"
                      << "\n  .position = " << token.position
                      << "\n  .literal  = " << std::quoted(token.literal)
                      << "\n  .type     = "
                      << tokenizer::get_token_t_name(token.type) << "\n}"
                      << std::endl;
    } catch (const libparser::errors::file_error& ex) {
        std::cerr << "tokenizer file error: " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "unhandled exception: " << ex.what() << std::endl;
    }

    return 0;
}
```

```cmake
#
# CMakeLists.txt
#

cmake_minimum_required(VERSION 3.24)

project(
    example
    LANGUAGES CXX
)

#
# Dependencies
#

include(FetchContent)

FetchContent_Declare(
    tokenizer
    GIT_REPOSITORY git@github.com:AndreaGiorgino/libtokenizer
    GIT_TAG 1.0.0
)

FetchContent_MakeAvailable(tokenizer)

#
# Target
#

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_executable(
    ${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cxx
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC tokenizer
)
```
