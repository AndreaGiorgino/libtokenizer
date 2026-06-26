#pragma once

/*
 * libtokenizer - version 1.0.0-alpha
 */

#include <filesystem>
#include <generator>
#include <string>

#include "libparser/parser.hxx"

namespace libtokenizer {
    /**
     * @class tokenizer
     * @brief Represents the tokenizer
     *
     */
    class tokenizer final {
        public: // definitions
            /**
             * @enum token_t
             * @brief Enumerates the token types
             */
            enum class token_t {
                UNDEFINED,

                // syntax
                END_OF_FILE,
                NEWLINE,
                SPACE,

                // symbols
                AMP,
                AT,
                BACKSLASH,
                BANG,
                BAR,
                CLOSE_BRACKET,
                CLOSE_PAREN,
                CLOSE_SQUARE,
                COLON,
                COMMA,
                DASH,
                DOT,
                DOUBLE_QUOTE,
                EQUALS,
                GREATHER,
                HASH,
                LESS,
                OPEN_BRACKET,
                OPEN_PAREN,
                OPEN_SQUARE,
                PERC,
                PLUS,
                QUESTION,
                QUOTE,
                SEMICOLON,
                SLASH,
                STAR,
                UNDERSCORE,

                // other
                ALPHANUMERIC,
                NUMBER,
                STRING_DOUBLE_QUOTED,
                STRING_QUOTED,
            };

            /**
             * @class token
             * @brief Represents the parsed part
             *
             */
            struct token final {
                std::streamoff position { -1 };
                std::string literal {};
                token_t type {};
            };

        public: // ctors
            tokenizer(std::string_view sourceFilePath);

            tokenizer(const tokenizer&) = delete;
            auto operator =(const tokenizer&) -> tokenizer& = delete;

            tokenizer(tokenizer&&) = delete;
            auto operator =(tokenizer&&) -> tokenizer& = delete;

            ~tokenizer(void) = default;

        private: // members
            std::filesystem::path _sourceFilePath {};
            std::unique_ptr<libparser::parser> _parser { nullptr };
            token _bufferedToken {};
    };
} // namespace libtokenizer
