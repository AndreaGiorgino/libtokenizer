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

   public:
    // Ctors ///////////////////////////////////////////////////////////////////
    Tokenizer(void) noexcept = delete;
    Tokenizer(std::istream& is, bool collapseStrings = true);

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
