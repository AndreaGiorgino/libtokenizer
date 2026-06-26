#include <iostream>

#include "libtokenizer/tokenizer.hxx"

static const auto testFilePath { "TestFiles/TestOpenFile.txt" };

auto TestOpenFile(int, char**) -> int {
    try {
        libtokenizer::tokenizer t { testFilePath };
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        return 1;
    }

    return 0;
}
