#include <sstream>

#include "utility.hxx"

auto Test_StreamNavigation(int, char**) -> int {
    std::stringstream ss {"token1 token2"};
    Tokenizer tokenizer {
        ss, Tokenizer::OptionsSet {}.set(Tokenizer::Options::IgnoreSpaces)};

    expectToken(tokenizer.peek(), "token1", Tokenizer::TokenType::Identifier,
                __LINE__);
    expectToken(tokenizer.peek(), "token1", Tokenizer::TokenType::Identifier,
                __LINE__);
    expectToken(tokenizer.get(), "token1", Tokenizer::TokenType::Identifier,
                __LINE__);
    expectToken(tokenizer.peek(), "token2", Tokenizer::TokenType::Identifier,
                __LINE__);
    expectToken(tokenizer.get(), "token2", Tokenizer::TokenType::Identifier,
                __LINE__);

    return 0;
}
