#include "filepath.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"


#define FILEPATH_TEST(scene_name, p, dir, base)                                                                        \
    TEST(scene_name)                                                                                                   \
    {                                                                                                                  \
        char* t = NULL;                                                                                                \
                                                                                                                       \
        printf("%s\n", p);                                                                                             \
                                                                                                                       \
        t = strdup(p);                                                                                                 \
        char* b = basename(t);                                                                                         \
        ASSERT(0 == strcmp(base, b));                                                                                  \
        free(t);                                                                                                       \
                                                                                                                       \
        t = strdup(p);                                                                                                 \
        char* d = dirname(t);                                                                                          \
        ASSERT(0 == strcmp(dir, d));                                                                                   \
        free(t);                                                                                                       \
    }

FILEPATH_TEST(Test_root, "/", "/", "/")
FILEPATH_TEST(Test_normal, "/usr/lib", "/usr", "lib")
FILEPATH_TEST(Test_end_with_pathspr, "/usr/", "/", "usr")
FILEPATH_TEST(Test_file_in_root, "/usr", "/", "usr")
FILEPATH_TEST(Test_no_pathspr, "usr", ".", "usr")
FILEPATH_TEST(Test_cur, ".", ".", ".")
FILEPATH_TEST(Test_parent, "..", ".", "..")
FILEPATH_TEST(Test_relative, "usr/tm.f", "usr", "tm.f")
FILEPATH_TEST(Test_more_pathspr, "/usr///", "/", "usr")
