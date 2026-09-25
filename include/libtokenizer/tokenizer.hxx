#pragma once
#ifndef H_TOKENIZER
#define H_TOKENIZER

#include <bitset>
#include <istream>

class Tokenizer final {
   public:
    /*
     * @brief Represents the token types
     */
    enum class TokenType {
        None,

        Eof,
        NewLine,
        Space,

        Identifier,
        NumericFloat,
        NumericInt,
        Symbol,

        StringBacktickQuote,
        StringDoubleQuote,
        StringSingleQuote,
    };

    /*
     * @brief Represents a token
     */
    struct Token final {
        std::streamoff offset {};
        std::string literal {};
        TokenType type {};
    };

    /*
     * @brief Represents the tokenizer options
     */
    enum Options {
        /*
         * @brief Allow dashes to be part of identifiers
         *
         * Allowed examples:
         *   - "some_var"
         *   - "__some_var"
         *   - "some_var__"
         *
         * Not allowed examples:
         *   - "_" will parse the underscore as a token
         *   - "_-" will parse the trailing dash as a separate token
         */
        AllowDashIdentifier,

        /*
         * @brief Allow underscores to be part of identifiers
         *
         * Allowed examples:
         *   - "some-var"
         *
         * Not allowed examples:
         *   - "-some-var" will parse the leading dash as a separate token
         *   - "some-var-" will parse the trailing dash as a separate token
         */
        AllowUnderscoreIdentifier,

        /*
         * @brief Allow underscores to be part of numeric tokens
         *
         * Allowed examples:
         *   - "1_000"
         *   - "1_0_0"
         *
         * Not allowed examples:
         *   - "_1" will parse the leading underscore as a separate token
         *   - "1_" will parse the trailing underscore as a separate token
         */
        AllowUnderscoreNumeric,

        /*
         * @brief Treat multiple matching spaces as one token
         *
         * Allowed examples:
         *   - "\s\s"
         *   - "\t\t"
         *
         * Not allowed examples:
         *   - "\s\t"
         */
        CollapseSpaces,

        /*
         * @brief Treat quotation marks as the start of a TokenType::String*
         */
        CollapseStrings,

        /*
         * @brief Do not return TokenType::NewLine and TokenType::SPACE
         */
        IgnoreSpaces,

        __Count__,
    };

    /*
     * @brief Represents the bitset containing the options
     */
    using OptionsSet = std::bitset<Options::__Count__>;

   public:
    // Ctors ///////////////////////////////////////////////////////////////////
    Tokenizer(void) noexcept = delete;
    Tokenizer(std::istream& is,
              OptionsSet options = OptionsSet {}
                                       .set(Options::AllowUnderscoreIdentifier)
                                       .set(Options::AllowUnderscoreNumeric)
                                       .set(Options::CollapseSpaces)
                                       .set(Options::CollapseStrings));

    // Copy ///////////////////////////////////////////////////////////////////
    Tokenizer(const Tokenizer&) noexcept                     = delete;
    auto operator =(const Tokenizer&) noexcept -> Tokenizer& = delete;

    // Move ///////////////////////////////////////////////////////////////////
    Tokenizer(Tokenizer&&) noexcept                     = delete;
    auto operator =(Tokenizer&&) noexcept -> Tokenizer& = delete;

    // Dtor ///////////////////////////////////////////////////////////////////
    ~Tokenizer(void) noexcept = default;

   public:
    // Modifiers //////////////////////////////////////////////////////////////
    /**
     * @brief Get the next token
     */
    [[nodiscard]] auto get(void) -> Token;

    /**
     * @brief Peek the next token
     */
    [[nodiscard]] auto peek(void) -> Token;

    /**
     * @brief Get the stream reading position
     */
    [[nodiscard]] auto tellg(void) -> std::streamoff;

    /**
     * @brief Set the stream reading position
     *
     * @param offset The offset from the beginning of the stream
     */
    auto seekg(std::streamoff offset) -> void;

    /**
     * @brief Check wether the stream has reached EOF
     */
    [[nodiscard]] auto eof(void) -> bool;

   private:
    std::istream& _is;
    OptionsSet _options {};

    Token _bufferedToken {.type = TokenType::None};
};

#endif
