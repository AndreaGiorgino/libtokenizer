#include <iostream>

#include "libtokenizer/tokenizer.hxx"

static constexpr auto testFilePath { "TestFiles/TestParseFileStream.txt" };

auto TestParseFileStreamEof(int, char**) -> int {
    libtokenizer::tokenizer t { testFilePath };

    {
        const auto isEof { t.eof() };
        if (isEof) {
            std::println(std::cerr,
                    "-- EOF not matching the expected result: "
                    "{} expected, got {} instead (streamoff: {}).",
                    false, isEof, t.tellg());
            return 1;
        }
    }

    while (!t.eof())
        t.ignore();

    {
        const auto isEof { t.eof() };
        if (!isEof) {
            std::println(std::cerr,
                    "-- EOF not matching the expected result: "
                    "{} expected, got {} instead (streamoff: {}).",
                    true, isEof, t.tellg());
            return 1;
        }
    }

    return 0;
}
