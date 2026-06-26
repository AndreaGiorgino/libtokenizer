#pragma once

#include <string>

namespace libtokenizer::errors {
    class tokenizer_error final : public std::exception {
        public: // ctors
            tokenizer_error(std::string_view errorMessage = "Unknown error") noexcept;

            tokenizer_error(const tokenizer_error&) noexcept = default;
            auto operator =(const tokenizer_error&) noexcept -> tokenizer_error& = default;

            tokenizer_error(tokenizer_error&&) noexcept = default;
            auto operator =(tokenizer_error&&) noexcept -> tokenizer_error& = default;

            ~tokenizer_error(void) noexcept = default;

        public:
            auto what(void) const noexcept -> const char* override;

        private:
            std::string _errorMessage {};
    };
} // namespace libtokenizer::errors
