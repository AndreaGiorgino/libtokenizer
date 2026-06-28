#include <array>
#include <iostream>

#include "libtokenizer/tokenizer.hxx"

static constexpr auto testFilePath { "TestFiles/TestParseFileGenerator.txt" };
static constexpr std::array<libtokenizer::tokenizer::token, 50> tokensResult {{
    { 0,   "#", libtokenizer::tokenizer::token_t::HASH },
    { 1,   "include", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 8,   " ", libtokenizer::tokenizer::token_t::SPACE },
    { 9,   "<", libtokenizer::tokenizer::token_t::LESS },
    { 10,  "print", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 15,  ">", libtokenizer::tokenizer::token_t::GREATHER },
    { 16,  "\n", libtokenizer::tokenizer::token_t::NEWLINE },
    { 17,  "\n", libtokenizer::tokenizer::token_t::NEWLINE },
    { 18,  "auto", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 22,  " ", libtokenizer::tokenizer::token_t::SPACE },
    { 23,  "main", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 27,  "(", libtokenizer::tokenizer::token_t::OPEN_PAREN },
    { 28,  "int", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 31,  " ", libtokenizer::tokenizer::token_t::SPACE },
    { 32,  "argc", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 36,  ",", libtokenizer::tokenizer::token_t::COMMA },
    { 37,  " ", libtokenizer::tokenizer::token_t::SPACE },
    { 38,  "char", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 42,  "*", libtokenizer::tokenizer::token_t::STAR },
    { 43,  "*", libtokenizer::tokenizer::token_t::STAR },
    { 44,  " ", libtokenizer::tokenizer::token_t::SPACE },
    { 45,  "argv", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 49,  ")", libtokenizer::tokenizer::token_t::CLOSE_PAREN },
    { 50,  " ", libtokenizer::tokenizer::token_t::SPACE },
    { 51,  "-", libtokenizer::tokenizer::token_t::DASH },
    { 52,  ">", libtokenizer::tokenizer::token_t::GREATHER },
    { 53,  " ", libtokenizer::tokenizer::token_t::SPACE },
    { 54,  "int", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 57,  " ", libtokenizer::tokenizer::token_t::SPACE },
    { 58,  "{", libtokenizer::tokenizer::token_t::OPEN_BRACKET },
    { 59,  "\n", libtokenizer::tokenizer::token_t::NEWLINE },
    { 60,  "    ", libtokenizer::tokenizer::token_t::SPACE },
    { 64,  "std", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 67,  ":", libtokenizer::tokenizer::token_t::COLON },
    { 68,  ":", libtokenizer::tokenizer::token_t::COLON },
    { 69,  "println", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 76,  "(", libtokenizer::tokenizer::token_t::OPEN_PAREN },
    { 77,  "\"Hello, world!\"", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 92,  ")", libtokenizer::tokenizer::token_t::CLOSE_PAREN },
    { 93,  ";", libtokenizer::tokenizer::token_t::SEMICOLON },
    { 94,  "\n", libtokenizer::tokenizer::token_t::NEWLINE },
    { 95,  "    ", libtokenizer::tokenizer::token_t::SPACE },
    { 99,  "return", libtokenizer::tokenizer::token_t::ALPHANUMERIC },
    { 105, " ", libtokenizer::tokenizer::token_t::SPACE },
    { 106, "0", libtokenizer::tokenizer::token_t::NUMERIC },
    { 107, ";", libtokenizer::tokenizer::token_t::SEMICOLON },
    { 108, "\n", libtokenizer::tokenizer::token_t::NEWLINE },
    { 109, "}", libtokenizer::tokenizer::token_t::CLOSE_BRACKET },
    { 110, "\n", libtokenizer::tokenizer::token_t::NEWLINE },
    { 111, "\x{ff}", libtokenizer::tokenizer::token_t::END_OF_FILE },
}};

auto TestParseFileGenerator(int, char**) -> int {
    libtokenizer::tokenizer t { testFilePath };

    size_t i {};
    for (const auto& token : t.tokens()) {
        if (i >= tokensResult.size()) {
            std::println(std::cerr,
                    "-- Token index out of bounds: accessing index {} "
                    "out of {} expected tokens.",
                    i, tokensResult.size());
            return 1;
        }

        if (const auto& tokenResult { tokensResult[i++] };
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
    }

    return 0;
}

