#include <array>
#include <iostream>

#include "libtokenizer/tokenizer.hxx"

static constexpr auto testFilePath { "TestFiles/TestParseFileStream.txt" };
static constexpr std::array<libtokenizer::tokenizer::token, 2> tokensResult {{
    { 0, "#", libtokenizer::tokenizer::token_t::HASH },
    { 1, "include", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
}};

auto TestParseFileStreamGet(int, char**) -> int {
    libtokenizer::tokenizer t { testFilePath };

    const auto lambda {
        [&](size_t resultIndex) -> int {
            const auto token { t.get() };
            if (const auto& tokenResult { tokensResult[resultIndex] };
                    token.position != tokenResult.position
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

    for (size_t i {}; i < tokensResult.size(); i++)
        if (const auto ret { lambda(i) };
                ret != 0)
            return ret;

    return 0;
}
