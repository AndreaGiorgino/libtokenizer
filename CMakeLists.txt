cmake_minimum_required(VERSION 3.11)

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
    GIT_TAG 2.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(parser)

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
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE $<TARGET_FILE_DIR:parser>/include
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE parser
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PostBuild.cmake)
