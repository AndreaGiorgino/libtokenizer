#pragma once
#ifndef H_TOKENIZER
#define H_TOKENIZER

#include <istream>

class Tokenizer final {
   public:
    /*
     * @brief Represents the token types
     */
    enum class TokenType {
        NONE,

        END_OF_FILE,
        NEWLINE,
        SPACE,

        IDENTIFIER,
        NUMERIC_FLOAT,
        NUMERIC_INT,
        SYMBOL,

        STRING_BACKTICK,
        STRING_DOUBLE_QUOTE,
        STRING_SINGLE_QUOTE,
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
        ALLOW_DASH_IDENTIFIER = 1 << 0,

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
        ALLOW_UNDERSCORE_IDENTIFIER = 1 << 1,

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
        ALLOW_UNDERSCORE_NUMERIC = 1 << 2,

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
        COLLAPSE_SPACES = 1 << 3,

        /*
         * @brief Treat quotation marks as the start of a TokenType::STRING_*
         */
        COLLAPSE_STRINGS = 1 << 4,

        /*
         * @brief Do not return TokenType::NEWLINE and TokenType::SPACE
         */
        IGNORE_SPACES = 1 << 5,
    };

   public:
    // Ctors ///////////////////////////////////////////////////////////////////
    Tokenizer(void) noexcept = delete;
    Tokenizer(std::istream& is, uint8_t options = ALLOW_UNDERSCORE_IDENTIFIER
                                                  | ALLOW_UNDERSCORE_NUMERIC
                                                  | COLLAPSE_SPACES
                                                  | COLLAPSE_STRINGS);

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
    [[nodiscard]] auto get(void) -> Token;

    [[nodiscard]] auto peek(void) -> Token;

    [[nodiscard]] auto tellg(void) -> std::streamoff;

    auto seekg(std::streamoff offset) -> void;

    [[nodiscard]] auto eof(void) -> bool;

   private:
    std::istream& _is;
    uint8_t _options {};

    Token _bufferedToken {.type = TokenType::NONE};
};

#endif
