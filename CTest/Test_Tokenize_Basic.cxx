#include <sstream>

#include "utility.hxx"

auto Test_Tokenize_Basic(int, char**) -> int {
    std::stringstream ss {"var_name 1_234 3.14159 #"};
    Tokenizer tokenizer {ss,
                         Tokenizer::OptionsSet {}
                             .set(Tokenizer::Options::AllowUnderscoreIdentifier)
                             .set(Tokenizer::Options::AllowUnderscoreNumeric)
                             .set(Tokenizer::Options::CollapseSpaces)
                             .set(Tokenizer::Options::CollapseStrings)
                             .set(Tokenizer::Options::IgnoreSpaces)};

    expectToken(tokenizer.get(), "var_name", Tokenizer::TokenType::Identifier,
                __LINE__);
    expectToken(tokenizer.get(), "1_234", Tokenizer::TokenType::NumericInt,
                __LINE__);
    expectToken(tokenizer.get(), "3.14159", Tokenizer::TokenType::NumericFloat,
                __LINE__);
    expectToken(tokenizer.get(), "#", Tokenizer::TokenType::Symbol, __LINE__);
    expectToken(tokenizer.get(), "", Tokenizer::TokenType::Eos, __LINE__);

    return 0;
}
