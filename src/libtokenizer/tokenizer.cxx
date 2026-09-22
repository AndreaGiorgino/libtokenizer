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

// Definitions ////////////////////////////////////////////////////////////////

auto tryParseIdentifier(std::istream& is, uint8_t options) noexcept
    -> Tokenizer::Token {
    assert(!is.eof() && "invalid call: stream should not be eof");

    const auto prefix {is.peek()};

    assert((prefix == '_' || prefix == '-' || std::isalpha(prefix))
           && "invalid call: first character is not an identifier prefix");

    if (prefix == '_'
        && !(options & Tokenizer::Options::ALLOW_UNDERSCORE_IDENTIFIER))
        return {
            .offset  = is.tellg(),
            .literal = std::string {static_cast<char>(is.get())},
            .type    = Tokenizer::TokenType::SYMBOL,
        };
    else if (prefix == '-'
             && !(options & Tokenizer::Options::ALLOW_DASH_IDENTIFIER))
        return {
            .offset  = is.tellg(),
            .literal = std::string {static_cast<char>(is.get())},
            .type    = Tokenizer::TokenType::SYMBOL,
        };

    const auto offset {is.tellg()};
    std::string buffer {};

    int ch {};

    do {
        ch = is.peek();

        if (ch == '-' && (options & Tokenizer::Options::ALLOW_DASH_IDENTIFIER))
            buffer += is.get();
        else if (ch == '_'
                 && (options & Tokenizer::Options::ALLOW_UNDERSCORE_IDENTIFIER))
            buffer += is.get();
        else if (std::isalnum(ch))
            buffer += is.get();
        else
            break;
    } while (!is.eof());

    if (buffer.size() == 1 && !std::isalpha(buffer[0]))
        return {
            .offset  = offset,
            .literal = buffer,
            .type    = Tokenizer::TokenType::SYMBOL,
        };

    return {
        .offset  = offset,
        .literal = buffer,
        .type    = Tokenizer::TokenType::IDENTIFIER,
    };
}

auto tryParseNumeric(std::istream& is, uint8_t options) noexcept
    -> Tokenizer::Token {
    assert(!is.eof() && "invalid call: stream should not be eof");
    assert(std::isdigit(is.peek())
           && "invalid call: first character is not a numeric prefix");

    const auto offset {is.tellg()};
    std::string buffer {};

    int ch {};
    bool isNumericFloat {};

    do {
        ch = is.peek();

        if (ch == '_'
            && (options & Tokenizer::Options::ALLOW_UNDERSCORE_NUMERIC))
            buffer += is.get();
        else if (ch == '.' && !isNumericFloat) {
            isNumericFloat = true;
            buffer += is.get();
        } else if (std::isdigit(ch))
            buffer += is.get();
        else
            break;
    } while (!is.eof());

    return {
        .offset  = offset,
        .literal = buffer,
        .type    = isNumericFloat ? Tokenizer::TokenType::NUMERIC_FLOAT
                                  : Tokenizer::TokenType::NUMERIC_INT,
    };
}

auto tryParseString(std::istream& is, uint8_t options) noexcept
    -> Tokenizer::Token {
    assert(!is.eof() && "invalid call: stream should not be eof");

    const auto offset {is.tellg()};
    const auto quote {is.get()};

    assert((quote == '\'' || quote == '"' || quote == '`')
           && "invalid call: first character is not a valid quotation mark");

    if (!(options & Tokenizer::Options::COLLAPSE_STRINGS))
        return {
            .offset  = offset,
            .literal = std::string {static_cast<char>(quote)},
            .type    = Tokenizer::TokenType::SYMBOL,
        };

    const auto type {[&] {
        if (quote == '\'')
            return Tokenizer::TokenType::STRING_SINGLE_QUOTE;
        else if (quote == '"')
            return Tokenizer::TokenType::STRING_SINGLE_QUOTE;
        else if (quote == '`')
            return Tokenizer::TokenType::STRING_SINGLE_QUOTE;

        std::unreachable();
    }()};

    std::string buffer {static_cast<char>(quote)};

    int ch {};
    bool isClosed {};

    do {
        ch = is.get();
        buffer += ch;

        if (ch == quote && buffer[buffer.size() - 1] != '\\') {
            isClosed = true;
            break;
        } else if (ch == '\n')
            break;
    } while (!is.eof());

    if (!isClosed) {
        buffer = static_cast<char>(quote);
        is.seekg((int)offset + 1);

        return {
            .offset  = offset,
            .literal = std::string {static_cast<char>(quote)},
            .type    = Tokenizer::TokenType::SYMBOL,
        };
    }

    return {
        .offset  = offset,
        .literal = buffer,
        .type    = type,
    };
}
    : _is(is),
      _collapseStrings(collapseStrings) {
    if (!is)
        throw std::runtime_error("invalid stream provided");
}
auto Tokenizer::peek(void) -> Token {
    const auto offset {_is.tellg()};

    (void)get();
    _is.seekg(offset);

    return _bufferedToken;
}

auto Tokenizer::tellg(void) -> std::streamoff {
    return _is.tellg();
}

auto Tokenizer::seekg(std::streamoff offset) -> void {
    _is.seekg(offset);
}

auto Tokenizer::eof(void) -> bool {
    return _is.eof();
}
