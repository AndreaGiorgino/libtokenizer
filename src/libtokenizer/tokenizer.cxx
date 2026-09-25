#include <cassert>
#include <libtokenizer/tokenizer.hxx>
#include <utility>

// Declarations ///////////////////////////////////////////////////////////////

[[nodiscard]] auto tryParseIdentifier(std::istream& is,
                                      Tokenizer::OptionsSet options) noexcept
    -> Tokenizer::Token;

[[nodiscard]] auto tryParseString(std::istream& is,
                                  Tokenizer::OptionsSet options) noexcept
    -> Tokenizer::Token;

[[nodiscard]] auto tryParseNumeric(std::istream& is,
                                   Tokenizer::OptionsSet options) noexcept
    -> Tokenizer::Token;

// Definitions ////////////////////////////////////////////////////////////////

auto tryParseIdentifier(std::istream& is,
                        Tokenizer::OptionsSet options) noexcept
    -> Tokenizer::Token {
    assert(!is.eof() && "invalid call: stream should not be eof");

    const auto prefix {is.peek()};

    assert((prefix == '_' || std::isalpha(prefix))
           && "invalid call: first character is not an identifier prefix");

    if (prefix == '_'
        && !options.test(Tokenizer::Options::AllowUnderscoreIdentifier))
        // parse the underscore as a separate symbol
        return {
            .offset  = is.tellg(),
            .literal = std::string {static_cast<char>(is.get())},
            .type    = Tokenizer::TokenType::Symbol,
        };

    const auto offset {is.tellg()};
    std::string buffer {};

    int ch {};

    do {
        ch = is.peek();

        if (ch == '-' && options.test(Tokenizer::Options::AllowDashIdentifier))
            buffer += is.get();
        else if (ch == '_'
                 && options.test(Tokenizer::Options::AllowUnderscoreIdentifier))
            buffer += is.get();
        else if (std::isalnum(ch))
            buffer += is.get();
        else
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
            .type    = Tokenizer::TokenType::Symbol,
        };

    return {
        .offset  = offset,
        .literal = buffer,
        .type    = Tokenizer::TokenType::Identifier,
    };
}

auto tryParseNumeric(std::istream& is, Tokenizer::OptionsSet options) noexcept
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
            && options.test(Tokenizer::Options::AllowUnderscoreNumeric))
            buffer += is.get();
        else if (ch == '.' && !isNumericFloat) {
            isNumericFloat = true;
            buffer += is.get();
        } else if (std::isdigit(ch))
            buffer += is.get();
        else
            break;
    } while (!is.eof());

    for (int i {static_cast<int>(buffer.size()) - 1}; i >= 0; i--)
        // pop trailing underscores
        if (buffer[i] == '_')
            buffer.pop_back();
        else
            break;

    // reset stream offset after buffer updates
    is.seekg(static_cast<int>(offset) + buffer.size());

    return {
        .offset  = offset,
        .literal = buffer,
        .type    = isNumericFloat ? Tokenizer::TokenType::NumericFloat
                                  : Tokenizer::TokenType::NumericInt,
    };
}

auto tryParseString(std::istream& is, Tokenizer::OptionsSet options) noexcept
    -> Tokenizer::Token {
    assert(!is.eof() && "invalid call: stream should not be eof");

    const auto offset {is.tellg()};
    const auto quote {is.get()};

    assert((quote == '\'' || quote == '"' || quote == '`')
           && "invalid call: first character is not a valid quotation mark");

    if (!options.test(Tokenizer::Options::CollapseStrings))
        // treat the quotation mark as a separate symbol
        return {
            .offset  = offset,
            .literal = std::string {static_cast<char>(quote)},
            .type    = Tokenizer::TokenType::Symbol,
        };

    const auto type {[&] {
        // set the quotation type
        if (quote == '\'')
            return Tokenizer::TokenType::StringSingleQuote;
        else if (quote == '"')
            return Tokenizer::TokenType::StringSingleQuote;
        else if (quote == '`')
            return Tokenizer::TokenType::StringSingleQuote;

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
            .type    = Tokenizer::TokenType::Symbol,
        };
    }

    return {
        .offset  = offset,
        .literal = buffer,
        .type    = type,
    };
}

Tokenizer::Tokenizer(std::istream& is, OptionsSet options) : _is(is) {
    if (!is)
        throw std::runtime_error("invalid stream provided");

    _options = options;
}

auto Tokenizer::get(void) -> Token {
    if (_bufferedToken.type != TokenType::None
        && _is.tellg() == _bufferedToken.offset)
        return _bufferedToken;

    const auto ch {_is.peek()};

    if (ch == EOF)
        return _bufferedToken = {
                   .offset  = _is.tellg(),
                   .literal = {},
                   .type    = TokenType::Eos,
        };
    else if (std::isspace(ch)) {
        if (_options.test(Options::IgnoreSpaces)) {
            while (std::isspace(_is.peek()))
                _is.ignore();

            return get();
        }

        const auto offset {_is.tellg()};
        std::string buffer {static_cast<char>(_is.get())};

        if (_options.test(Options::CollapseSpaces))
            while (_is.peek() == ch)
                buffer += _is.get();

        return _bufferedToken = {
                   .offset  = offset,
                   .literal = buffer,
                   .type = (ch == '\n' ? TokenType::NewLine : TokenType::Space),
        };
    } else if (std::ispunct(ch)) {
        if (ch == '_')
            return _bufferedToken = tryParseIdentifier(_is, _options);
        else if (ch == '\'' || ch == '"' || ch == '`')
            return _bufferedToken = tryParseString(_is, _options);

        return _bufferedToken = {
                   .offset  = _is.tellg(),
                   .literal = std::string {static_cast<char>(_is.get())},
                   .type    = TokenType::Symbol,
        };
    } else if (std::isdigit(ch))
        return _bufferedToken = tryParseNumeric(_is, _options);
    else if (std::isalpha(ch))
        return _bufferedToken = tryParseIdentifier(_is, _options);

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
