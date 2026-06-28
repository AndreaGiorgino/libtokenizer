#include <iostream>

#include "libtokenizer/tokenizer.hxx"

static constexpr auto testFilePath { "TestFiles/TestParseFileStream.txt" };
static constexpr libtokenizer::tokenizer::token tokenResult {
    0, "#", libtokenizer::tokenizer::token_t::HASH
};

auto TestParseFileStreamSeek(int, char**) -> int {
    libtokenizer::tokenizer t { testFilePath };

    const auto lambda {
        [&](void) -> int {
            const auto token { t.get() };
            if (token.position != tokenResult.position
                    || token.literal != tokenResult.literal
                    || token.type != tokenResult.type) {
                std::println(std::cerr,
                        "-- Token not matching the expected result: "
                        "\"{{ {}, {:?}, {} }}\" expected, got "
                        "\"{{ {}, {:?}, {} }}\" instead.",
                        tokenResult.position, tokenResult.literal,
                        libtokenizer::tokenizer::get_token_t_name(tokenResult.type),
                        token.position, token.literal,
                        libtokenizer::tokenizer::get_token_t_name(token.type));
                return 1;
            }
            return 0;
        }
    };

    if (const auto ret { lambda() };
            ret != 0)
        return ret;

    t.seekg(0);

    if (const auto ret { lambda() };
            ret != 0)
        return ret;

    return 0;
}
