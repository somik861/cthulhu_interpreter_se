#include "parser_dummy.h"

namespace cthu::parser {
/* static */ std::unique_ptr<IParser> IParser::createParserDummy() { return std::make_unique<impl::ParserDummy>(); }
} // namespace cthu::parser
