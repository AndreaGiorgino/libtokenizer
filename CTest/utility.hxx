#pragma once
#ifndef H_UTILITY
#define H_UTILITY

#include <libtokenizer/tokenizer.hxx>
#include <utility>

static auto getTokenTypeName(Tokenizer::TokenType type) -> std::string {
    switch (type) {
        case Tokenizer::TokenType::None:
            return "NONE";
        case Tokenizer::TokenType::Eos:
            return "EOS";
        case Tokenizer::TokenType::NewLine:
            return "NEWLINE";
        case Tokenizer::TokenType::Space:
            return "SPACE";
        case Tokenizer::TokenType::Identifier:
            return "IDENTIFIER";
        case Tokenizer::TokenType::NumericFloat:
            return "NUMERIC_FLOAT";
        case Tokenizer::TokenType::NumericInt:
            return "NUMERIC_INT";
        case Tokenizer::TokenType::Symbol:
            return "SYMBOL";
        case Tokenizer::TokenType::StringBacktick:
            return "STRING_BACKTICK";
        case Tokenizer::TokenType::StringDoubleQuote:
            return "STRING_DOUBLE_QUOTE";
        case Tokenizer::TokenType::StringSingleQuote:
            return "STRING_SINGLE_QUOTE";
    }

    std::unreachable();
}

static auto expectToken(Tokenizer::Token token, std::string_view literal,
                        Tokenizer::TokenType type, int line) -> void {
    if (token.literal != literal)
        throw std::runtime_error(
            std::format("literal mismatch at line {}: got {:?} instead of {:?}",
                        line, token.literal, literal));
    else if (token.type != type)
        throw std::runtime_error(std::format(
            "type mismatch at line {}: got {:?} instead of {:?}", line,
            getTokenTypeName(token.type), getTokenTypeName(type)));
}

#endif
