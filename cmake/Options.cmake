option(DEBUG "Enable debug build." OFF)
option(TOKENIZER_BUILD_TESTING "Enable testing and build tests." OFF)

if(DEBUG)
    add_compile_definitions(DEBUG)
    add_compile_options(-g -ggdb -O0)
endif()

if(TOKENIZER_BUILD_TESTING)
    enable_testing()
    add_subdirectory(ctest)
endif()
