#include <sstream>

#include "utility.hxx"

auto Test_Tokenize_IdentifierOptions(int, char**) -> int {
    {
        // no options
        std::stringstream ss {"some-var  1_1"};
        Tokenizer tokenizer {ss, {}};

        expectToken(tokenizer.get(), "some", Tokenizer::TokenType::Identifier,
                    __LINE__);
        expectToken(tokenizer.get(), "-", Tokenizer::TokenType::Symbol,
                    __LINE__);
        expectToken(tokenizer.get(), "var", Tokenizer::TokenType::Identifier,
                    __LINE__);
        expectToken(tokenizer.get(), " ", Tokenizer::TokenType::Space,
                    __LINE__);
        expectToken(tokenizer.get(), " ", Tokenizer::TokenType::Space,
                    __LINE__);
        expectToken(tokenizer.get(), "1", Tokenizer::TokenType::NumericInt,
                    __LINE__);
        expectToken(tokenizer.get(), "_", Tokenizer::TokenType::Symbol,
                    __LINE__);
        expectToken(tokenizer.get(), "1", Tokenizer::TokenType::NumericInt,
                    __LINE__);
        expectToken(tokenizer.get(), "", Tokenizer::TokenType::Eos, __LINE__);
    }

    {
        // AllowDashIdentifier
        std::stringstream ss {"some-var"};
        Tokenizer tokenizer {ss, Tokenizer::OptionsSet {}.set(
                                     Tokenizer::Options::AllowDashIdentifier)};

        expectToken(tokenizer.get(), "some-var",
                    Tokenizer::TokenType::Identifier, __LINE__);
        expectToken(tokenizer.get(), "", Tokenizer::TokenType::Eos, __LINE__);
    }
                    __LINE__);
        expectToken(tokenizer.get(), "1", Tokenizer::TokenType::NumericInt,
                    __LINE__);
        expectToken(tokenizer.get(), "_", Tokenizer::TokenType::Symbol,
                    __LINE__);
        expectToken(tokenizer.get(), "1", Tokenizer::TokenType::NumericInt,
                    __LINE__);
        expectToken(tokenizer.get(), "", Tokenizer::TokenType::Eos, __LINE__);
    }

    return 0;
}
