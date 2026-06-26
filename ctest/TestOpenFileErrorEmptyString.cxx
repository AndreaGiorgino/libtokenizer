#include <iostream>

#include "libtokenizer/tokenizer.hxx"
#include "libtokenizer/errors/file_error.hxx"

static constexpr auto testFilePath { "" };

auto TestOpenFileErrorEmptyString(int, char**) -> int {
    try {
        libtokenizer::tokenizer t { testFilePath };
    } catch (const libtokenizer::errors::file_error& err) {
        return 0;
    } catch (const std::exception& err){
        std::cerr << err.what() << std::endl;
        return 1;
    }

    return 0;
}
