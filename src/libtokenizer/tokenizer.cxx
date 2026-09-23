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

    assert((prefix == '_' || std::isalpha(prefix))
           && "invalid call: first character is not an identifier prefix");

    if (prefix == '_'
        && !(options & Tokenizer::Options::ALLOW_UNDERSCORE_IDENTIFIER))
        // parse the underscore as a separate symbol
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
            // include dashes
            buffer += is.get();
        else if (ch == '_'
                 && (options & Tokenizer::Options::ALLOW_UNDERSCORE_IDENTIFIER))
            // include underscores
            buffer += is.get();
        else if (std::isalnum(ch))
            buffer += is.get();
        else
            // do not include not-alphanumeric characters
            break;
    } while (!is.eof());

    for (int i {static_cast<int>(buffer.size()) - 1}; i >= 0; i--)
        // pop trailing dashes
        if (buffer[i] == '-')
            buffer.pop_back();
        else
            break;

    // reset stream offset after buffer updates
    is.seekg(static_cast<int>(offset) + buffer.size());

    if (buffer.size() == 1 && buffer[0] == '_')
        // check whether only an underscore remained
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
            // include underscores
            buffer += is.get();
        else if (ch == '.' && !isNumericFloat) {
            // include the first dot
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
        // treat the quotation mark as a separate symbol
        return {
            .offset  = offset,
            .literal = std::string {static_cast<char>(quote)},
            .type    = Tokenizer::TokenType::SYMBOL,
        };

    const auto type {[&] {
        // set the quotation type
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
            // close the quotation
            isClosed = true;
            break;
        } else if (ch == '\n')
            break;
    } while (!is.eof());

    if (!isClosed) {
        // unclosed string, treat the quotation mark as a separate symbol and
        // reset the stream offset
        is.seekg(static_cast<int>(offset) + 1);

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

Tokenizer::Tokenizer(std::istream& is, uint8_t options)
    : _is(is),
      _options(options) {
    if (!is)
        throw std::runtime_error("invalid stream provided");
}

auto Tokenizer::get(void) -> Token {
    if (_bufferedToken.type != TokenType::NONE
        && _is.tellg() == _bufferedToken.offset)
        return _bufferedToken;
    else if (_is.eof())
        return _bufferedToken = {
                   .offset  = _is.tellg(),
                   .literal = {},
                   .type    = TokenType::END_OF_FILE,
        };

    const auto ch {_is.peek()};

    if (std::isspace(ch)) {
        if (_options & Options::IGNORE_SPACES) {
            while (std::isspace(_is.peek()))
                _is.ignore();
            return get();
        }

        std::string buffer {};

        if (_options & Options::COLLAPSE_SPACES)
            while (_is.peek() == ch)
                buffer += _is.get();

        return _bufferedToken = {
                   .offset  = _is.tellg(),
                   .literal = buffer,
                   .type    = TokenType::NEWLINE,
        };
    } else if (std::ispunct(ch)) {
        if (ch == '_' || ch == '-')
            return _bufferedToken = tryParseIdentifier(_is, _options);
        else if (ch == '\'' || ch == '"' || ch == '`')
            return _bufferedToken = tryParseString(_is, _options);

        return _bufferedToken = {
                   .offset  = _is.tellg(),
                   .literal = std::string {static_cast<char>(ch)},
                   .type    = TokenType::SYMBOL,
        };
    } else if (std::isdigit(ch))
        return _bufferedToken = tryParseNumeric(_is, _options);
    else if (std::isalpha(ch))
        return _bufferedToken = tryParseIdentifier(_is, _options);
    else
        std::unreachable();
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
