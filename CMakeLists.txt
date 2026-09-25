cmake_minimum_required(VERSION 3.24)

project(
    libtokenizer
    VERSION 2.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# Dependencies
#

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Options.cmake)

#
# Target
#

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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/PostBuild.cmake)

if(LIBTOKENIZER_BUILD_EXAMPLE)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example)
endif()
