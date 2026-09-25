#include <sstream>

#include "utility.hxx"

auto Test_Tokenize_Empty(int, char**) -> int {
    std::stringstream ss {};
    Tokenizer tokenizer {ss};

    expectToken(tokenizer.get(), "", Tokenizer::TokenType::Eos, __LINE__);

    return 0;
}
