#pragma once
#ifndef H_UTILITY
#define H_UTILITY

#include <format>
#include <libtokenizer/tokenizer.hxx>
#include <utility>

static auto getTokenTypeName(Tokenizer::TokenType type) -> std::string {
    switch (type) {
        case Tokenizer::TokenType::None:
            return "None";
        case Tokenizer::TokenType::Eof:
            return "Eos";
        case Tokenizer::TokenType::NewLine:
            return "NewLine";
        case Tokenizer::TokenType::Space:
            return "Space";
        case Tokenizer::TokenType::Identifier:
            return "Identifier";
        case Tokenizer::TokenType::NumericFloat:
            return "NumericFloat";
        case Tokenizer::TokenType::NumericInt:
            return "NumericInt";
        case Tokenizer::TokenType::Symbol:
            return "Symbol";
        case Tokenizer::TokenType::StringBacktickQuote:
            return "StringBacktick";
        case Tokenizer::TokenType::StringSingleQuote:
            return "StringSingleQuote";
        case Tokenizer::TokenType::StringDoubleQuote:
            return "StringDoubleQuote";
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
