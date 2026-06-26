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
                NUMERIC,
                SYMBOL,
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

        public: // methods
            /**
             * @brief Get the token type name
             *
             * @param type The token type to get the name of
             */
            [[nodiscard]]
                static auto get_token_t_name(token_t type) -> std::string;

            /**
             * @brief Get the next token
             */
            [[nodiscard]]
                auto get(void) -> token;

            /**
             * @brief Peek the next token
             */
            [[nodiscard]]
                auto peek(void) -> token;

            /**
             * @brief Ignore the next tokens
             *
             * @param count The count of tokens to ignore
             */
            auto ignore(size_t count = 1) -> void;

            /**
             * @brief Get the stream offset of the file being parsed
             */
            [[nodiscard]]
                auto tellg(void) -> std::streamoff;

            /**
             * @brief Set the stream offset of the file being parsed
             *
             * @param streamoff The offset to set the file stream to
             */
            auto seekg(std::streamoff streamoff) -> void;

            /**
             * @brief Get if the file stream has reached eof
             */
            [[nodiscard]]
                auto eof(void) const -> bool;

            /**
             * @brief Get the tokens in a sequence
             */
            [[nodiscard]]
                auto tokens(void) -> std::generator<token>;

        private: // members
            std::filesystem::path _sourceFilePath {};
            std::unique_ptr<libparser::parser> _parser { nullptr };
            token _bufferedToken {};
    };
} // namespace libtokenizer
