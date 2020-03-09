#ifndef TOKEN_PARSER_TEST_C
#define TOKEN_PARSER_TEST_C

#include "token.h"

TEST(Test_token)
{
    struct token_cache_t cache = {TOKEN_CACHE_CONTENT_LEN_DEF, TOKEN_CACHE_FORWARD_LEN_DEF};

    struct token_reader_t reader = {
        "",
    };

    struct token_parser_t parser;
    token_parser_init(&parser, &reader, &cache);

    defer();

    token_parser_accept_space(&parser);
}
#endif  // TOKEN_PARSER_TEST_C
