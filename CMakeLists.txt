cmake_minimum_required(VERSION 3.5)

project(utils LANGUAGES C)

SET(UT_SRC
    .clang-format
    thirdparty/cmocka-ext/src/cmocka.c
    )


add_executable(line_reader
    #${UT_SRC}
    exports.h
    line_reader.h
    line_reader.c
    line_reader_test.c
    )

add_executable(token
    #${UT_SRC}
    token.h
    token.c
    token_test.c
    )

add_executable(hash_map
    #${UT_SRC}
    hash_map.h
    hash_map.c
    hash.h
    hash_map_str_int.h
    hash_map_str_int.c
    hash_map_test.c
    )


add_executable(panic
    panic.h
    panic_test2.c
    )
target_include_directories(panic PRIVATE
    thirdparty/cmocka-ext/include
    )


add_executable(defer
    defer.h
    defer_test1.c
    )

add_executable(filepath
    filepath.h
    filepath.c
    filepath_test.c
    )
