cmake_minimum_required(VERSION 3.5)

project(utils LANGUAGES C)

add_executable(line_reader .clang-format exports.h line_reader.h line_reader.c line_reader_test.c)

add_executable(token token.h token.c token_test.c)

add_executable(hash_map hash_map.h hash_map.c hash_map_test.c)

add_executable(panic panic.h panic_test2.c)

add_executable(defer defer.h defer_test1.c)

add_executable(filepath filepath.h filepath.c filepath_test.c)
