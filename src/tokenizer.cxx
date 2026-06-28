#include <format>
#include <unordered_map>

#include "errors/tokenizer_error.hxx"
#include "libparser/errors/file_error.hxx"
#include "tokenizer.hxx"

namespace fs = std::filesystem;

namespace libtokenizer {
    static const std::unordered_map<char, tokenizer::token_t> token_t_map {{
        { '\n', tokenizer::token_t::NEWLINE },
        { ' ',  tokenizer::token_t::SPACE   },
        { '&',  tokenizer::token_t::AMP },
        { '@',  tokenizer::token_t::AT },
        { '\\', tokenizer::token_t::BACKSLASH },
        { '!',  tokenizer::token_t::BANG },
        { '|',  tokenizer::token_t::BAR },
        { '}',  tokenizer::token_t::CLOSE_BRACKET },
        { ')',  tokenizer::token_t::CLOSE_PAREN },
        { ']',  tokenizer::token_t::CLOSE_SQUARE },
        { ':',  tokenizer::token_t::COLON },
        { ',',  tokenizer::token_t::COMMA },
        { '-',  tokenizer::token_t::DASH },
        { '.',  tokenizer::token_t::DOT },
        { '"',  tokenizer::token_t::DOUBLE_QUOTE },
        { '=',  tokenizer::token_t::EQUALS },
        { '>',  tokenizer::token_t::GREATHER },
        { '#',  tokenizer::token_t::HASH },
        { '<',  tokenizer::token_t::LESS },
        { '{',  tokenizer::token_t::OPEN_BRACKET },
        { '(',  tokenizer::token_t::OPEN_PAREN },
        { '[',  tokenizer::token_t::OPEN_SQUARE },
        { '%',  tokenizer::token_t::PERC },
        { '+',  tokenizer::token_t::PLUS },
        { '?',  tokenizer::token_t::QUESTION },
        { '\'', tokenizer::token_t::QUOTE },
        { ';',  tokenizer::token_t::SEMICOLON },
        { '/',  tokenizer::token_t::SLASH },
        { '*',  tokenizer::token_t::STAR },
        { '_',  tokenizer::token_t::UNDERSCORE },
    }};

    auto tokenizer::get_token_t_name(tokenizer::token_t type) -> std::string {
        switch (type) {
            case tokenizer::token_t::END_OF_FILE:          return "END_OF_FILE";
            case tokenizer::token_t::NEWLINE:              return "NEWLINE";
            case tokenizer::token_t::SPACE:                return "SPACE";
            case tokenizer::token_t::AMP:                  return "AMP";
            case tokenizer::token_t::AT:                   return "AT";
            case tokenizer::token_t::BACKSLASH:            return "BACKSLASH";
            case tokenizer::token_t::BANG:                 return "BANG";
            case tokenizer::token_t::BAR:                  return "BAR";
            case tokenizer::token_t::CLOSE_BRACKET:        return "CLOSE_BRACKET";
            case tokenizer::token_t::CLOSE_PAREN:          return "CLOSE_PAREN";
            case tokenizer::token_t::CLOSE_SQUARE:         return "CLOSE_SQUARE";
            case tokenizer::token_t::COLON:                return "COLON";
            case tokenizer::token_t::COMMA:                return "COMMA";
            case tokenizer::token_t::DASH:                 return "DASH";
            case tokenizer::token_t::DOT:                  return "DOT";
            case tokenizer::token_t::DOUBLE_QUOTE:         return "DOUBLE_QUOTE";
            case tokenizer::token_t::EQUALS:               return "EQUALS";
            case tokenizer::token_t::GREATHER:             return "GREATHER";
            case tokenizer::token_t::HASH:                 return "HASH";
            case tokenizer::token_t::LESS:                 return "LESS";
            case tokenizer::token_t::OPEN_BRACKET:         return "OPEN_BRACKET";
            case tokenizer::token_t::OPEN_PAREN:           return "OPEN_PAREN";
            case tokenizer::token_t::OPEN_SQUARE:          return "OPEN_SQUARE";
            case tokenizer::token_t::PERC:                 return "PERC";
            case tokenizer::token_t::PLUS:                 return "PLUS";
            case tokenizer::token_t::QUESTION:             return "QUESTION";
            case tokenizer::token_t::QUOTE:                return "QUOTE";
            case tokenizer::token_t::SEMICOLON:            return "SEMICOLON";
            case tokenizer::token_t::SLASH:                return "SLASH";
            case tokenizer::token_t::STAR:                 return "STAR";
            case tokenizer::token_t::UNDERSCORE:           return "UNDERSCORE";
            case tokenizer::token_t::ALPHANUMERIC:         return "ALPHANUMERIC";
            case tokenizer::token_t::NUMERIC:              return "NUMERIC";
            case tokenizer::token_t::SYMBOL:               return "SYMBOL";
            case tokenizer::token_t::STRING_DOUBLE_QUOTED: return "STRING_DOUBLE_QUOTED";
            case tokenizer::token_t::STRING_QUOTED:        return "STRING_QUOTED";
            default:
               throw libtokenizer::errors::tokenizer_error(
                       std::format(
                           "Token type name not defined: {}",
                           std::to_underlying(type)));
        };
    }

    tokenizer::tokenizer(std::string_view sourceFilePath) {
        if (sourceFilePath.empty())
            throw libparser::errors::file_error("Cannot use an empty path");

        _sourceFilePath = fs::absolute(sourceFilePath);

        try {
            _parser = std::make_unique<libparser::parser>(sourceFilePath);
        } catch (const libparser::errors::file_error& err) {
            throw err;
        } catch (const std::exception& err) {
            throw libtokenizer::errors::tokenizer_error(err.what());
        }
    }

    auto tokenizer::get(void) -> token {
        if (_parser->tellg() == _bufferedToken.position) {
            _parser->seekg(_bufferedToken.literal.size());
            return _bufferedToken;
        }

        const auto& pToken { _parser->get() };

        _bufferedToken = {
            pToken.position,
            pToken.literal,
            token_t::ALPHANUMERIC,
        };

        // check for newlines
        if (_bufferedToken.literal == "\n") {
            _bufferedToken.type = token_t::NEWLINE;
            return _bufferedToken;
        }

        if (pToken.literal == "'"
                || pToken.literal == "\"") {
            // handle quoted strings
            const auto quotationMark { pToken.literal };

            libparser::parser::token next {};
            while (!_parser->eof()) {
                next = _parser->get();
                _bufferedToken.literal += next.literal;

                // check for newline
                if (next.literal == "\n")
                    throw errors::tokenizer_error(
                            std::format(
                                "Unclosed quoted string at position {}: {:?}.",
                                _bufferedToken.position, _bufferedToken.literal));

                // checkfor quotation end
                if (next.literal == quotationMark)
                    break;
            }

            // check for eof
            if (*(_bufferedToken.literal.end() - 1) != quotationMark[0])
                throw errors::tokenizer_error(
                        std::format(
                            "Unclosed quoted string at position {}: {:?}.",
                            _bufferedToken.position, _bufferedToken.literal));
            else return _bufferedToken;
        } else if (pToken.literal.size() == 1) {
            // handle symbols
            const auto& ch { pToken.literal[0] };

            // check for EOF
            if (ch == EOF) {
                _bufferedToken.type = tokenizer::token_t::END_OF_FILE;
                return _bufferedToken;
            }

            // check for known symbols
            if (const auto it { token_t_map.find(ch) };
                    it != token_t_map.end())
                _bufferedToken.type = it->second;

            // unknown symbol
            _bufferedToken.type = tokenizer::token_t::SYMBOL;
            return _bufferedToken;
        }

        // check for numeric token
        bool testFlag { true };
        for (const char ch : _bufferedToken.literal)
            if (!std::isdigit(ch)) {
                testFlag = false;
                break;
            }

        if (testFlag) {
            _bufferedToken.type = token_t::NUMERIC;
            return _bufferedToken;
        }

        // test for space token
        testFlag = true;
        for (const char ch : _bufferedToken.literal)
            if (!std::isspace(ch)) {
                testFlag = false;
                break;
            }

        if (testFlag) {
            _bufferedToken.type = token_t::SPACE;
            return _bufferedToken;
        }

        // alphanumeric token
        return _bufferedToken;
    }

    auto tokenizer::peek(void) -> token {
        (void)get();

        _parser->seekg(_bufferedToken.position - 1);
        return _bufferedToken;
    }

    auto tokenizer::ignore(size_t count) -> void {
        for (size_t i {}; i < count; i++)
            (void)get();
    }

    auto tokenizer::tellg(void) -> std::streamoff {
        return _parser->tellg();
    }

    auto tokenizer::seekg(std::streamoff streamoff) -> void {
        _parser->seekg(streamoff);
    }

    auto tokenizer::eof(void) const -> bool {
        return _parser->eof();
    }

    auto tokenizer::tokens(void) -> std::generator<token> {
        while (!_parser->eof())
            co_yield get();
    }
} // namespace libtokenizer
