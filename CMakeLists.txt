cmake_minimum_required(VERSION 3.24)

project(
    tokenizer
    VERSION 1.0.0
    LANGUAGES CXX
)

#
# Dependencies
#

include(FetchContent)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Options.cmake)

FetchContent_Declare(
    parser
    GIT_REPOSITORY git@github.com:AndreaGiorgino/libparser.git
    GIT_TAG 2.1.1
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(parser)

#
# Target
#

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(
    GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx
)

add_library(
    ${PROJECT_NAME}
    STATIC ${SOURCE_FILES}
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE -Wextra -Wall -pedantic
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:$<TARGET_FILE_DIR:${PROJECT_NAME}>/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC parser
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PostBuild.cmake)
