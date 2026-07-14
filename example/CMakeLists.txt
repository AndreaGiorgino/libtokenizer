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
