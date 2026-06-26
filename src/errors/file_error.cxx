#include <format>

#include "errors/file_error.hxx"

namespace libtokenizer::errors {
    file_error::file_error(std::string_view errorMessage) noexcept {
        _errorMessage = std::format("-- File error: {}", errorMessage);
    }

    auto file_error::what(void) const noexcept -> const char* {
        return _errorMessage.data();
    }
} // namespace libtokenizer::errors
