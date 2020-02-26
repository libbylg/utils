cmake_minimum_required(VERSION 3.5)

project(utils LANGUAGES C)

add_executable(utils
    exports.h

    main.c

    line_reader.h
    line_reader.c
    line_reader_test.c

    token.h
    token.c
    token_test.c

    hash_map.h
    hash_map.c
    hash_map_test.c

    panic.h
    panic_test2.c

    defer.h
    defer_test1.c

    filepath.h
    filepath.c
    filepath_test.c
    )
