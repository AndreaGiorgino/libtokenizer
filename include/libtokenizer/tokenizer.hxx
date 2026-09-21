
#pragma once
#ifndef H_TOKENIZER
#define H_TOKENIZER

namespace libtokenizer {
class tokenizer final {
   public:
    // ------------------------------------------------------------------------
    // Ctors
    // ------------------------------------------------------------------------
    tokenizer(void) noexcept = default;

    // ------------------------------------------------------------------------
    // Copy
    // ------------------------------------------------------------------------
    tokenizer(const tokenizer&) noexcept                     = default;
    auto operator =(const tokenizer&) noexcept -> tokenizer& = default;

    // ------------------------------------------------------------------------
    // Move
    // ------------------------------------------------------------------------
    tokenizer(tokenizer&&) noexcept                     = default;
    auto operator =(tokenizer&&) noexcept -> tokenizer& = default;

    // ------------------------------------------------------------------------
    // Dtor
    // ------------------------------------------------------------------------
    ~tokenizer(void) noexcept = default;
};
} // namespace libtokenizer

#endif
