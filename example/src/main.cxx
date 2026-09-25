#include <libtokenizer/tokenizer.hxx>
#include <print>
#include <sstream>
#include <utility>

auto main(int, char**) -> int {
    std::stringstream ss {
        R"(#include <iostream>

auto main(int, char**) -> int {
    std::cout << "Hello world" << std::endl;
    return 0;
})"};

    Tokenizer tokenizer {ss};
    Tokenizer::Token token {};

    do {
        token = tokenizer.get();
        std::println("[offset = {:3}, type = {}]: {:?}", token.offset,
                     std::to_underlying(token.type), token.literal);
    } while (token.type != Tokenizer::TokenType::Eos);

    return 0;
}
