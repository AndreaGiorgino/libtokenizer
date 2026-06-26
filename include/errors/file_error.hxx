#pragma once

#include <string>

namespace libtokenizer::errors {
    class file_error final : public std::exception {
        public: // ctors
            file_error(std::string_view errorMessage = "Unknown error") noexcept;

            file_error(const file_error&) noexcept = default;
            auto operator =(const file_error&) noexcept -> file_error& = default;

            file_error(file_error&&) noexcept = default;
            auto operator =(file_error&&) noexcept -> file_error& = default;

            ~file_error(void) noexcept = default;

        public:
            auto what(void) const noexcept -> const char* override;

        private:
            std::string _errorMessage {};
    };
} // namespace libtokenizer::errors
