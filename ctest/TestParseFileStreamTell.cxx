#include <iostream>

#include "libtokenizer/tokenizer.hxx"

static constexpr auto testFilePath { "TestFiles/TestParseFileStream.txt" };
static constexpr auto positionResult { 2 };

auto TestParseFileStreamTell(int, char**) -> int {
    libtokenizer::tokenizer t { testFilePath };

    t.ignore();

    const auto position { t.tellg() };
    const auto token { t.get() };
    if (token.position != position) {
        std::println(std::cerr,
                "-- Position not matching the expected result: "
                "{} expected, got {} instead.",
                positionResult, position);
        return 1;
    }

    return 0;
}
