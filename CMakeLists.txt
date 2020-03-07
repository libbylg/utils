cmake_minimum_required(VERSION 3.5)


project(utils LANGUAGES C)


SET(UT_SRC
    test.h
    test.c
    )


#SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -DTEST_AUTO_RUN)


#add_executable(test
#    ${UT_SRC}
#    test_test.c
#    )


add_executable(line_reader
    #${UT_SRC}
    exports.h
    line_reader.h
    line_reader.c
    line_reader_test.c
    )


#add_executable(token
#    #${UT_SRC}
#    token.h
#    token.c
#    token_test.c
#    )


#add_executable(hash_map
#    ${UT_SRC}
#    hash_map.h
#    hash_map.c
#    hash.h
#    hash_map_str_int.h
#    hash_map_str_int.c
#    hash_map_test.c
#    )


#add_executable(list_head
#    ${UT_SRC}
#    list.h
#    list_test.c
#    )


#add_executable(panic
#    panic.h
#    panic_test.c
#    )
#target_include_directories(panic PRIVATE
#    thirdparty/cmocka-ext/include
#    )


#add_executable(defer
#    .clang-format
#    defer.h
#    defer_test.c
#    test.h
#    test.c
#    str.c
#    str.h
#    str_test.c
#    )

#target_include_directories(defer PRIVATE
#    thirdparty/cmocka-ext/include
#    )

#add_executable(filepath
#    filepath.h
#    filepath.c
#    filepath_test.c
#    )


#add_executable(str
#    ${UT_SRC}
#    str.h
#    str.c
#    str_test.c
#    )
