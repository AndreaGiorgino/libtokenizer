#include <format>
#include <iostream>
#include <print>

#include "libparser/errors/file_error.hxx"
#include "libtokenizer/tokenizer.hxx"

#define ANSI_BOLD  "\033[1m"
#define ANSI_RESET "\033[0m"

using namespace libtokenizer;

auto main(void) -> int {
    static constexpr auto filepath {"example.txt"};

    static constexpr auto posColWidth {4};
    static constexpr auto typeColWidth {15};

    static constexpr auto separatorWidth {40};
    static const std::string separator {
        std::format("{:->{}}", "", separatorWidth)};

    try {
        std::cout << separator << std::endl;
        std::println(ANSI_BOLD " {: <{}}" ANSI_RESET " | " ANSI_BOLD
                               "{: <{}}" ANSI_RESET " | " ANSI_BOLD
                               "Literal" ANSI_RESET,
            "Pos", posColWidth, "Type", typeColWidth);
        std::cout << separator << std::endl;

        tokenizer t {filepath};

        for (const auto& token : t.tokens())
            std::println(" {: >{}} | {: <{}} | {:?}", token.position,
                posColWidth, tokenizer::get_token_t_name(token.type),
                typeColWidth, token.literal);

        std::cout << separator << std::endl;
    } catch (const libparser::errors::file_error& ex) {
        std::cerr << "tokenizer file error: " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "unhandled exception: " << ex.what() << std::endl;
    }

    return 0;
}
