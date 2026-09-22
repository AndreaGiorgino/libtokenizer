#pragma once
#ifndef H_TOKENIZER
#define H_TOKENIZER

#include <istream>

class Tokenizer final {
   public:
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

    struct Token final {
        std::streamoff offset {};
        std::string literal {};
        TokenType type {};
    };

    enum Options {
        ALLOW_DASH_IDENTIFIER       = 1 << 0,
        ALLOW_UNDERSCORE_IDENTIFIER = 1 << 1,
        ALLOW_UNDERSCORE_NUMERIC    = 1 << 2,
        COLLAPSE_SPACES             = 1 << 3,
        COLLAPSE_STRINGS            = 1 << 4,
        IGNORE_SPACES               = 1 << 5,
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
    // Lookup /////////////////////////////////////////////////////////////////
    // Modifiers //////////////////////////////////////////////////////////////
    [[nodiscard]] auto get(void) -> Token;

    [[nodiscard]] auto peek(void) -> Token;

    [[nodiscard]] auto tellg(void) -> std::streamoff;

    auto seekg(std::streamoff offset) -> void;

    [[nodiscard]] auto eof(void) -> bool;

   private:
    std::istream& _is;
    bool _collapseStrings {false};

    Token _buffer {.type = TokenType::NONE};
};

#endif
