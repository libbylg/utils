//
//  behavior_parser.cpp
//  BehaviorAnalysis
//
//  Created by luolijun on 2020/1/25.
//  Copyright © 2020 baidu. All rights reserved.
//
#include "token.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "panic.h"

#ifndef ASSERT
#define ASSERT assert
#endif  // ASSERT

// clang-format off
enum CHAR_TYPE : int {
    CT_DIGIT    = 0x01,
    CT_NAME     = 0x02,    //  大小写字母,数字,点,中划线,下划线,
    CT_NEWLINE  = 0x04, //  换行符号
    CT_SPACE    = 0x08,
    CT_OPSPLIT  = 0x10, //  允许竖线和中划线
    CT_OPRCODE  = 0x20, //  只允许大小写字母和点
    CT_PMSPLI   = 0x40,  //  参数分隔符号: 中划线或者竖线
};
// clang-format on

static int default_char_maps[] = {
    /*  0    0x00         */ 0,
    /*  1    0x01         */ 0,
    /*  2    0x02         */ 0,
    /*  3    0x03         */ 0,
    /*  4    0x04         */ 0,
    /*  5    0x05         */ 0,
    /*  6    0x06         */ 0,
    /*  7    0x07         */ 0,
    /*  8    0x08         */ 0,
    /*  9    0x09         */ CT_SPACE,
    /* 10    0x0a    \n   */ CT_NEWLINE,
    /* 11    0x0b         */ CT_SPACE,
    /* 12    0x0c         */ 0,
    /* 13    0x0d         */ 0,
    /* 14    0x0e         */ 0,
    /* 15    0x0f         */ 0,
    /* 16    0x10         */ 0,
    /* 17    0x11         */ 0,
    /* 18    0x12         */ 0,
    /* 19    0x13         */ 0,
    /* 20    0x14         */ 0,
    /* 21    0x15         */ 0,
    /* 22    0x16         */ 0,
    /* 23    0x17         */ 0,
    /* 24    0x18         */ 0,
    /* 25    0x19         */ 0,
    /* 26    0x1a         */ 0,
    /* 27    0x1b         */ 0,
    /* 28    0x1c         */ 0,
    /* 29    0x1d         */ 0,
    /* 30    0x1e         */ 0,
    /* 31    0x1f         */ 0,
    /* 32    0x20         */ CT_SPACE,
    /* 33    0x21    !    */ 0,
    /* 34    0x22    "    */ 0,
    /* 35    0x23    #    */ 0,
    /* 36    0x24    $    */ 0,
    /* 37    0x25    %    */ 0,
    /* 38    0x26    &    */ 0,
    /* 39    0x27    '    */ 0,
    /* 40    0x28    (    */ 0,
    /* 41    0x29    )    */ 0,
    /* 42    0x2a    *    */ 0,
    /* 43    0x2b    +    */ 0,
    /* 44    0x2c    ,    */ 0,
    /* 45    0x2d    -    */ CT_NAME | CT_OPSPLIT | CT_PMSPLI,
    /* 46    0x2e    .    */ CT_NAME | CT_OPRCODE,
    /* 47    0x2f    /    */ 0,
    /* 48    0x30    0    */ CT_NAME | CT_DIGIT | CT_OPRCODE,
    /* 49    0x31    1    */ CT_NAME | CT_DIGIT | CT_OPRCODE,
    /* 50    0x32    2    */ CT_NAME | CT_DIGIT | CT_OPRCODE,
    /* 51    0x33    3    */ CT_NAME | CT_DIGIT | CT_OPRCODE,
    /* 52    0x34    4    */ CT_NAME | CT_DIGIT | CT_OPRCODE,
    /* 53    0x35    5    */ CT_NAME | CT_DIGIT | CT_OPRCODE,
    /* 54    0x36    6    */ CT_NAME | CT_DIGIT | CT_OPRCODE,
    /* 55    0x37    7    */ CT_NAME | CT_DIGIT | CT_OPRCODE,
    /* 56    0x38    8    */ CT_NAME | CT_DIGIT | CT_OPRCODE,
    /* 57    0x39    9    */ CT_NAME | CT_DIGIT | CT_OPRCODE,
    /* 58    0x3a    :    */ CT_OPSPLIT,
    /* 59    0x3b    ;    */ 0,
    /* 60    0x3c    <    */ 0,
    /* 61    0x3d    =    */ 0,
    /* 62    0x3e    >    */ 0,
    /* 63    0x3f    ?    */ 0,
    /* 64    0x40    @    */ 0,
    /* 65    0x41    A    */ CT_NAME | CT_OPRCODE,
    /* 66    0x42    B    */ CT_NAME | CT_OPRCODE,
    /* 67    0x43    C    */ CT_NAME | CT_OPRCODE,
    /* 68    0x44    D    */ CT_NAME | CT_OPRCODE,
    /* 69    0x45    E    */ CT_NAME | CT_OPRCODE,
    /* 70    0x46    F    */ CT_NAME | CT_OPRCODE,
    /* 71    0x47    G    */ CT_NAME | CT_OPRCODE,
    /* 72    0x48    H    */ CT_NAME | CT_OPRCODE,
    /* 73    0x49    I    */ CT_NAME | CT_OPRCODE,
    /* 74    0x4a    J    */ CT_NAME | CT_OPRCODE,
    /* 75    0x4b    K    */ CT_NAME | CT_OPRCODE,
    /* 76    0x4c    L    */ CT_NAME | CT_OPRCODE,
    /* 77    0x4d    M    */ CT_NAME | CT_OPRCODE,
    /* 78    0x4e    N    */ CT_NAME | CT_OPRCODE,
    /* 79    0x4f    O    */ CT_NAME | CT_OPRCODE,
    /* 80    0x50    P    */ CT_NAME | CT_OPRCODE,
    /* 81    0x51    Q    */ CT_NAME | CT_OPRCODE,
    /* 82    0x52    R    */ CT_NAME | CT_OPRCODE,
    /* 83    0x53    S    */ CT_NAME | CT_OPRCODE,
    /* 84    0x54    T    */ CT_NAME | CT_OPRCODE,
    /* 85    0x55    U    */ CT_NAME | CT_OPRCODE,
    /* 86    0x56    V    */ CT_NAME | CT_OPRCODE,
    /* 87    0x57    W    */ CT_NAME | CT_OPRCODE,
    /* 88    0x58    X    */ CT_NAME | CT_OPRCODE,
    /* 89    0x59    Y    */ CT_NAME | CT_OPRCODE,
    /* 90    0x5a    Z    */ CT_NAME | CT_OPRCODE,
    /* 91    0x5b    [    */ 0,
    /* 92    0x5c    \    */ 0,
    /* 93    0x5d    ]    */ 0,
    /* 94    0x5e    ^    */ 0,
    /* 95    0x5f    _    */ CT_NAME,
    /* 96    0x60    `    */ 0,
    /* 97    0x61    a    */ CT_NAME | CT_OPRCODE,
    /* 98    0x62    b    */ CT_NAME | CT_OPRCODE,
    /* 99    0x63    c    */ CT_NAME | CT_OPRCODE,
    /*100    0x64    d    */ CT_NAME | CT_OPRCODE,
    /*101    0x65    e    */ CT_NAME | CT_OPRCODE,
    /*102    0x66    f    */ CT_NAME | CT_OPRCODE,
    /*103    0x67    g    */ CT_NAME | CT_OPRCODE,
    /*104    0x68    h    */ CT_NAME | CT_OPRCODE,
    /*105    0x69    i    */ CT_NAME | CT_OPRCODE,
    /*106    0x6a    j    */ CT_NAME | CT_OPRCODE,
    /*107    0x6b    k    */ CT_NAME | CT_OPRCODE,
    /*108    0x6c    l    */ CT_NAME | CT_OPRCODE,
    /*109    0x6d    m    */ CT_NAME | CT_OPRCODE,
    /*110    0x6e    n    */ CT_NAME | CT_OPRCODE,
    /*111    0x6f    o    */ CT_NAME | CT_OPRCODE,
    /*112    0x70    p    */ CT_NAME | CT_OPRCODE,
    /*113    0x71    q    */ CT_NAME | CT_OPRCODE,
    /*114    0x72    r    */ CT_NAME | CT_OPRCODE,
    /*115    0x73    s    */ CT_NAME | CT_OPRCODE,
    /*116    0x74    t    */ CT_NAME | CT_OPRCODE,
    /*117    0x75    u    */ CT_NAME | CT_OPRCODE,
    /*118    0x76    v    */ CT_NAME | CT_OPRCODE,
    /*119    0x77    w    */ CT_NAME | CT_OPRCODE,
    /*120    0x78    x    */ CT_NAME | CT_OPRCODE,
    /*121    0x79    y    */ CT_NAME | CT_OPRCODE,
    /*122    0x7a    z    */ CT_NAME | CT_OPRCODE,
    /*123    0x7b    {    */ 0,
    /*124    0x7c    |    */ CT_OPSPLIT | CT_PMSPLI,
    /*125    0x7d    }    */ 0,
    /*126    0x7e    ~    */ 0,
    /*127    0x7f         */ 0,
    /*128    0x80         */ 0,
    /*129    0x81         */ 0,
    /*130    0x82         */ 0,
    /*131    0x83         */ 0,
    /*132    0x84         */ 0,
    /*133    0x85         */ 0,
    /*134    0x86         */ 0,
    /*135    0x87         */ 0,
    /*136    0x88         */ 0,
    /*137    0x89         */ 0,
    /*138    0x8a         */ 0,
    /*139    0x8b         */ 0,
    /*140    0x8c         */ 0,
    /*141    0x8d         */ 0,
    /*142    0x8e         */ 0,
    /*143    0x8f         */ 0,
    /*144    0x90         */ 0,
    /*145    0x91         */ 0,
    /*146    0x92         */ 0,
    /*147    0x93         */ 0,
    /*148    0x94         */ 0,
    /*149    0x95         */ 0,
    /*150    0x96         */ 0,
    /*151    0x97         */ 0,
    /*152    0x98         */ 0,
    /*153    0x99         */ 0,
    /*154    0x9a         */ 0,
    /*155    0x9b         */ 0,
    /*156    0x9c         */ 0,
    /*157    0x9d         */ 0,
    /*158    0x9e         */ 0,
    /*159    0x9f         */ 0,
    /*160    0xa0         */ 0,
    /*161    0xa1         */ 0,
    /*162    0xa2         */ 0,
    /*163    0xa3         */ 0,
    /*164    0xa4         */ 0,
    /*165    0xa5         */ 0,
    /*166    0xa6         */ 0,
    /*167    0xa7         */ 0,
    /*168    0xa8         */ 0,
    /*169    0xa9         */ 0,
    /*170    0xaa         */ 0,
    /*171    0xab         */ 0,
    /*172    0xac         */ 0,
    /*173    0xad         */ 0,
    /*174    0xae         */ 0,
    /*175    0xaf         */ 0,
    /*176    0xb0         */ 0,
    /*177    0xb1         */ 0,
    /*178    0xb2         */ 0,
    /*179    0xb3         */ 0,
    /*180    0xb4         */ 0,
    /*181    0xb5         */ 0,
    /*182    0xb6         */ 0,
    /*183    0xb7         */ 0,
    /*184    0xb8         */ 0,
    /*185    0xb9         */ 0,
    /*186    0xba         */ 0,
    /*187    0xbb         */ 0,
    /*188    0xbc         */ 0,
    /*189    0xbd         */ 0,
    /*190    0xbe         */ 0,
    /*191    0xbf         */ 0,
    /*192    0xc0         */ 0,
    /*193    0xc1         */ 0,
    /*194    0xc2         */ 0,
    /*195    0xc3         */ 0,
    /*196    0xc4         */ 0,
    /*197    0xc5         */ 0,
    /*198    0xc6         */ 0,
    /*199    0xc7         */ 0,
    /*200    0xc8         */ 0,
    /*201    0xc9         */ 0,
    /*202    0xca         */ 0,
    /*203    0xcb         */ 0,
    /*204    0xcc         */ 0,
    /*205    0xcd         */ 0,
    /*206    0xce         */ 0,
    /*207    0xcf         */ 0,
    /*208    0xd0         */ 0,
    /*209    0xd1         */ 0,
    /*210    0xd2         */ 0,
    /*211    0xd3         */ 0,
    /*212    0xd4         */ 0,
    /*213    0xd5         */ 0,
    /*214    0xd6         */ 0,
    /*215    0xd7         */ 0,
    /*216    0xd8         */ 0,
    /*217    0xd9         */ 0,
    /*218    0xda         */ 0,
    /*219    0xdb         */ 0,
    /*220    0xdc         */ 0,
    /*221    0xdd         */ 0,
    /*222    0xde         */ 0,
    /*223    0xdf         */ 0,
    /*224    0xe0         */ 0,
    /*225    0xe1         */ 0,
    /*226    0xe2         */ 0,
    /*227    0xe3         */ 0,
    /*228    0xe4         */ 0,
    /*229    0xe5         */ 0,
    /*230    0xe6         */ 0,
    /*231    0xe7         */ 0,
    /*232    0xe8         */ 0,
    /*233    0xe9         */ 0,
    /*234    0xea         */ 0,
    /*235    0xeb         */ 0,
    /*236    0xec         */ 0,
    /*237    0xed         */ 0,
    /*238    0xee         */ 0,
    /*239    0xef         */ 0,
    /*240    0xf0         */ 0,
    /*241    0xf1         */ 0,
    /*242    0xf2         */ 0,
    /*243    0xf3         */ 0,
    /*244    0xf4         */ 0,
    /*245    0xf5         */ 0,
    /*246    0xf6         */ 0,
    /*247    0xf7         */ 0,
    /*248    0xf8         */ 0,
    /*249    0xf9         */ 0,
    /*250    0xfa         */ 0,
    /*251    0xfb         */ 0,
    /*252    0xfc         */ 0,
    /*253    0xfd         */ 0,
    /*254    0xfe         */ 0,
    /*255    0xff         */ 0,
};

EXPORT_API int token_parser_init(struct token_parser_t* parser, struct token_reader_t* reader,
                                 struct token_cache_t* cache)
{
    ASSERT(NULL != parser);
    ASSERT(NULL != reader);
    ASSERT(NULL != cache);

    ASSERT(cache->content > 0);
    ASSERT(cache->forward > 1);
    ASSERT(cache->content >= (4 * cache->forward));

    parser->pos = parser->cache->buf;
    parser->end = parser->pos;
    parser->limit = parser->cache->buf + cache->content;
    parser->reader = reader;
    parser->end[0] = (uchar)('\n');
    parser->limit[0] = (uchar)('\n');

    return 0;
};

EXPORT_API void token_parser_destroy(struct token_parser_t* parser)
{
    ASSERT(NULL != parser);
}

//  bh_parser_fill_cache
//  函数仅仅用于填充缓冲区的数据，如果缓冲区的剩余空间小于特定的值，会尝试做一下数据空间搬移
EXPORT_API uchar* token_parser_fill_cache(struct token_parser_t* parser, int move_limit_size)
{
    //  当剩余数据较少时，做一次数据对齐
    int remain_data_len = (int)(parser->end - parser->pos);
    if (remain_data_len < move_limit_size) {
        if (remain_data_len > 0) {
            memmove(parser->cache, parser->pos, remain_data_len);
        }
        parser->pos = parser->cache->buf;
        parser->end = parser->pos + remain_data_len;
        parser->end[0] = '\n';
    }

    //  重新填充缓冲区
    int remain_buf_len = (int)(parser->limit - parser->end);
    int fill_len = parser->reader->read(parser->reader, parser->end, remain_buf_len);
    if (fill_len > 0) {
        parser->end += fill_len;
        parser->end[0] = '\n';
        return parser->pos;
    }

    if (fill_len == 0) {
        return parser->pos;
    }

    token_error_raise(&(parser->error), 1, "读取数据失败");
}

//  bh_parser_reserve 函数用于确保数据缓冲区至少有 reserve_size
//  个字节，除非已经没有更多数据。
EXPORT_API uchar* token_parser_reserve(struct token_parser_t* parser, int reserve_size)
{
    uchar* pc = parser->pos;
    int remain_data_len = (int)(parser->end - pc);

    //  如果缓冲区中剩余数据已经足够了，那么直接返回即可
    if (remain_data_len > reserve_size) {
        return pc;
    }

    //  如果剩余数据不多，那么做一下搬移，然后重新填充
    return token_parser_fill_cache(parser, reserve_size);
}

//  bh_accept_space 函数将跳过空白字符、空白行
EXPORT_API uchar* token_accept_space(struct token_parser_t* parser)
{
    uchar* pc = parser->pos;

TRY_AGAIN:
    while (default_char_maps[*pc] & CT_SPACE) {
        pc++;
    }

    if ((uchar)('\n') == pc[0]) {
        if (pc != parser->end) {
            pc++;
            goto TRY_AGAIN;
        } else {
            parser->pos = pc;  //  设置为全部识别完毕，避免不必要的数据迁移
            pc = token_parser_fill_cache(parser, parser->cache->head);

            //  如果重新载入一次数据后，仍然没有有效数据，说明缓冲区已经空了
            if (parser->pos != parser->end) {
                goto TRY_AGAIN;
            }
        }
    }

    parser->pos = pc;
    return pc;
}

EXPORT_API uchar* token_accept_token_pattern(struct token_parser_t* parser, int pattern)
{
    uchar* pc = token_parser_reserve(parser, parser->cache->ahead);
    while (default_char_maps[*pc] & (pattern)) {
        pc++;
    }

    if (parser->pos == pc) {
        token_error_raise(&(parser->error), 1, "不期望的符号(pattern=0x%X)", pattern);
    }

    return pc;
}

EXPORT_API uchar* token_accept_token_template(struct token_parser_t* parser, const char* tmpl)
{
    uchar* pc = token_parser_reserve(parser, parser->cache->ahead);
    while (*pc == (uchar)(*tmpl)) {
        tmpl++;
        pc++;
    }

    if ((uchar)('\0') == *tmpl) {
        return pc;
    }

    token_error_raise(&(parser->error), 1, "不是期望的符号 '%s'", tmpl);
}

EXPORT_API int token_error_vprintf(struct token_error_t* error, int id, const char* format, va_list va)
{
    ASSERT(NULL != error);
    error->id = id;
    return vsnprintf(error->error, sizeof(error->error), format, va);
}

EXPORT_API int token_error_printf(struct token_error_t* error, const char* format, ...)
{
    va_list va;
    va_start(va, format);
    int ret = vsnprintf(error->error, sizeof(error->error), format, va);
    va_end(va);
    return ret;
}

EXPORT_API void token_error_raise(struct token_error_t* error, int id, const char* format, ...)
{
    ASSERT(NULL != error);
    va_list va;
    va_start(va, format);
    error->id = id;
    (void)vsnprintf(error->error, sizeof(error->error), format, va);
    va_end(va);
    panic(error);
}
