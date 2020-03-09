cmake_minimum_required(VERSION 3.5)


project(utils LANGUAGES C)


SET(UT_SRC
    test.h
    test.c
    )


SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -DTEST_AUTO_RUN)


#add_executable(test
#    ${UT_SRC}
#    test_test.c
#    )


#add_executable(line_reader
#    #${UT_SRC}
#    exports.h
#


add_executable(utils
    ########
    test.h
    test.c
    #test_test.c

    line_reader.h
    line_reader.c
    line_reader_test.c

    ########
    exports.h
    macros.h

    ########
    bit_array.h
    bit_array_test.c

    ########
    token.h
    token.c
    token_test.c

    ########
    hash_map.h
    hash_map.c
    hash.h
    hash_map_str_int.h
    hash_map_str_int.c
    hash_map_test.c

    list.h
    list_test.c

    panic.h
    panic_test.c

    defer.h
    defer_test.c
    test.h
    test.c

    str.c
    str.h
    str_test.c

    filepath.h
    filepath.c
    filepath_test.c
    )

