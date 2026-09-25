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

    {
        // AllowUnderscoreIdentifier
        std::stringstream ss {"some_var"};
        Tokenizer tokenizer {
            ss, Tokenizer::OptionsSet {}.set(
                    Tokenizer::Options::AllowUnderscoreIdentifier)};

        expectToken(tokenizer.get(), "some_var",
                    Tokenizer::TokenType::Identifier, __LINE__);
        expectToken(tokenizer.get(), "", Tokenizer::TokenType::Eos, __LINE__);
    }

    {
        // AllowUnderscoreNumeric
        std::stringstream ss {"1_1"};
        Tokenizer tokenizer {ss,
                             Tokenizer::OptionsSet {}.set(
                                 Tokenizer::Options::AllowUnderscoreNumeric)};

        expectToken(tokenizer.get(), "1_1", Tokenizer::TokenType::NumericInt,
                    __LINE__);
        expectToken(tokenizer.get(), "", Tokenizer::TokenType::Eos, __LINE__);
    }

    {
        // CollapseSpaces
        std::stringstream ss {"  \t\t\n\n"};
        Tokenizer tokenizer {ss, Tokenizer::OptionsSet {}.set(
                                     Tokenizer::Options::CollapseSpaces)};

        expectToken(tokenizer.get(), "  ", Tokenizer::TokenType::Space,
                    __LINE__);
        expectToken(tokenizer.get(), "\t\t", Tokenizer::TokenType::Space,
                    __LINE__);
        expectToken(tokenizer.get(), "\n\n", Tokenizer::TokenType::NewLine,
                    __LINE__);
        expectToken(tokenizer.get(), "", Tokenizer::TokenType::Eos, __LINE__);
    }

    {
        // CollapseStrings
        std::stringstream ss {
            R"(`backtick quote` "double quote" 'single quote')"};
        Tokenizer tokenizer {ss, Tokenizer::OptionsSet {}
                                     .set(Tokenizer::Options::CollapseStrings)
                                     .set(Tokenizer::Options::IgnoreSpaces)};

        expectToken(tokenizer.get(), "`backtick quote`",
                    Tokenizer::TokenType::StringBacktick, __LINE__);
        expectToken(tokenizer.get(), R"("double quote")",
                    Tokenizer::TokenType::StringDoubleQuote, __LINE__);
        expectToken(tokenizer.get(), "'single quote'",
                    Tokenizer::TokenType::StringSingleQuote, __LINE__);
        expectToken(tokenizer.get(), "", Tokenizer::TokenType::Eos, __LINE__);
    }

    return 0;
}
