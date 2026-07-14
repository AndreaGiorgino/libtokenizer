#include <iostream>

#include "libparser/errors/file_error.hxx"
#include "libtokenizer/tokenizer.hxx"

using namespace libtokenizer;

auto main(void) -> int {
    static constexpr auto filepath {"example.txt"};

    try {
        tokenizer p {filepath};
        for (const auto& token : p.tokens())
            std::cout << "{"
                      << "\n  .position = " << token.position
                      << "\n  .literal  = " << std::quoted(token.literal)
                      << "\n  .type     = "
                      << tokenizer::get_token_t_name(token.type) << "\n}"
                      << std::endl;
    } catch (const libparser::errors::file_error& ex) {
        std::cerr << "tokenizer file error: " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "unhandled exception: " << ex.what() << std::endl;
    }

    return 0;
}
