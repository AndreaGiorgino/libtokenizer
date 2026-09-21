#include <libtokenizer/tokenizer.hxx>

Tokenizer::Tokenizer(std::istream& is, bool collapseStrings)
    : _is(is),
      _collapseStrings(collapseStrings) {
    if (!is)
        throw std::runtime_error("invalid stream provided");
}
