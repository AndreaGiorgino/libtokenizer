#include <format>

#include "errors/tokenizer_error.hxx"

namespace libtokenizer::errors {
    tokenizer_error::tokenizer_error(std::string_view errorMessage) noexcept {
        _errorMessage = std::format("-- Tokenizer error: {}", errorMessage);
    }

    auto tokenizer_error::what(void) const noexcept -> const char* {
        return _errorMessage.data();
    }
} // namespace libtokenizer::errors
