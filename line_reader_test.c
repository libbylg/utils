//
//  test_line_reader.c
//  BehaviorAnalysis
//
//  Created by baidu on 2020/2/16.
//  Copyright © 2020 baidu. All rights reserved.
//
#include "line_reader.h"

#include <stdio.h>

static int line_reader_scan_callback_echo(void* ctx, struct line_reader_t* r)
{
    printf("%.4ld [%s]\n", r->lino, r->line);
    return 0;
}

int main(int argc, char* argv[])
{
    const char* file =
        "/Users/baidu/baidu/svn/navi-qa-dev/tool/"
        "BehaviorAnalysis/BehaviorAnalysis/behavior-history/"
        "behavior_parser.cpp";
    struct line_reader_t r;

    // 初始化
    int ret = line_reader_init(&r, 4096);
    if (0 != ret) {
        return -1;
    }

    ret = line_reader_open(&r, file);
    if (0 != ret) {
        return -1;
    }

    // 逐行读取
    while (0 == line_reader_next(&r)) {
        int ret = line_reader_scan_callback_echo(NULL, &r);
        //  如果用户希望自行中断读取，那么就提前返回
        if (0 != ret) {
            break;
        }
    }

    // 销毁
    line_reader_destroy(&r);
    return 0;
}
