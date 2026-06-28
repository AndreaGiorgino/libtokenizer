#include <iostream>

#include "libtokenizer/tokenizer.hxx"

static constexpr auto testFilePath { "TestFiles/TestParseFileStream.txt" };
static constexpr libtokenizer::tokenizer::token tokenResult {
    9, "<", libtokenizer::tokenizer::token_t::LESS
};

auto TestParseFileStreamIgnoreMultiple(int, char**) -> int {
    libtokenizer::tokenizer t { testFilePath };

    t.ignore(3);

    const auto token { t.get() };
    if (token.position != tokenResult.position
            || token.literal != tokenResult.literal
            || token.type != tokenResult.type) {
        std::println(std::cerr,
                "-- Token not matching the expected result: "
                "\"{{ {}, {:?}, {} }}\" expected, "
                "got \"{{ {}, {:?}, {} }}\" instead.",
                tokenResult.position, tokenResult.literal,
                libtokenizer::tokenizer::get_token_t_name(token.type),
                token.position, token.literal,
                libtokenizer::tokenizer::get_token_t_name(tokenResult.type));
        return 1;
    }

    return 0;
}
