#include <libtokenizer/tokenizer.hxx>
#include <sstream>

auto Test_Ctor(int, char**) -> int {
    {
        // valid stream
        std::stringstream ss {};
        Tokenizer tokenizer {ss};
    }

    {
        // invalid stream
        std::stringstream ss {};
        ss.setstate(std::ios_base::failbit);

        try {
            Tokenizer tokenizer {ss};
            return 1;
        } catch (...) {
        }
    }

    return 0;
}
