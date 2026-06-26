#include <format>
#include <unordered_map>

#include "errors/file_error.hxx"
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

    tokenizer::tokenizer(std::string_view sourceFilePath) {
        _sourceFilePath = fs::absolute(sourceFilePath);

        try {
            _parser = std::make_unique<libparser::parser>(sourceFilePath);
        } catch (const libparser::errors::file_error& err) {
            throw libtokenizer::errors::file_error(err.what());
        } catch (const std::exception& err) {
            throw libtokenizer::errors::tokenizer_error(err.what());
        }
    }
} // namespace libtokenizer
