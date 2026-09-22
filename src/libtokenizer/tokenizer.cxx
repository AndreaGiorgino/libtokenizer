#include <cassert>
#include <libtokenizer/tokenizer.hxx>
#include <utility>

// Declarations ///////////////////////////////////////////////////////////////

[[nodiscard]] auto tryParseIdentifier(std::istream& is,
                                      uint8_t options) noexcept
    -> Tokenizer::Token;

[[nodiscard]] auto tryParseString(std::istream& is, uint8_t options) noexcept
    -> Tokenizer::Token;

[[nodiscard]] auto tryParseNumeric(std::istream& is, uint8_t options) noexcept
    -> Tokenizer::Token;
    : _is(is),
      _collapseStrings(collapseStrings) {
    if (!is)
        throw std::runtime_error("invalid stream provided");
}
