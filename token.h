//
//  behavior.hpp
//  BehaviorAnalysis
//  文件功能描述： 接口定义
//
//  Created by luolijun on 2020/1/25.
//  Copyright © 2020 baidu. All rights reserved.
//

#ifndef behavior_hpp
#define behavior_hpp

#include <stdarg.h>

#include "exports.h"

typedef unsigned char uchar;
typedef unsigned int uint;

//! `struct token_parser_error` defined a object to save to information of the
//! parser error.
struct token_error_t {
    int id;
    char error[500];
};
EXPORT_API void raise_token_error(struct token_error_t* error, int id, const char* format, ...);
EXPORT_API void raise_token_errorv(struct token_error_t* error, int id, const char* format, va_list va);

//!  `struct token_parser_reader_t` defined the interface to read more data
//!  from the data source.
struct token_reader_t {
    //! `ctx` is the context of this reader object
    void* ctx;

    //! @return
    //! If successful, the number of bytes actually read is returned.
    //! Return 0 will hit the end of the data, or return  -1 to to indicate the
    //! error.
    int (*read)(struct token_reader_t* reader, void* buf, int size);
};

#define TOKEN_CACHE_FORWARD_LEN_DEF (32)
#define TOKEN_CACHE_CONTENT_LEN_DEF \
    ((4096 - sizeof(void*)) - TOKEN_CACHE_FORWARD_LEN_DEF)
struct token_cache_t {
    int content;      //! The content length of the cache;
    int forward;      //! How many chars will be reserved, before fill the cache
    uchar buf[4096];  //! The buffer entry
};
#define TOKEN_CACHE_REALSIZE(cap, ahead) (sizeof(struct token_cache_t) - 4096)+ ((cap) + (ahead)) + sizeof(void*))

struct token_parser_t {
    uchar* pos;
    uchar* end;
    uchar* limit;
    uchar* line;
    long lino;
    uint* char_maps;
    struct token_error_t error;
    struct token_reader_t* reader;
    struct token_cache_t* cache;
};

EXPORT_API int token_parser_init(struct token_parser_t* parser,
                                 struct token_reader_t* reader,
                                 struct token_cache_t* cache);
EXPORT_API void token_parser_destroy(struct token_parser_t* parser);
EXPORT_API uchar* token_parser_fill_cache(struct token_parser_t* parser,
                                          int limit_size);
EXPORT_API uchar* token_parser_accept_space(struct token_parser_t* parser);
EXPORT_API uchar* token_parser_accept_token_pattern(
    struct token_parser_t* parser, int pattern);
EXPORT_API uchar* token_parser_accept_token_template(
    struct token_parser_t* parser, const char* tmpl);

#endif /* behavior_hpp */
