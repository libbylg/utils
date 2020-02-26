#include "filepath.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "cmocka.h"
#include "stdlib.h"

#define FILEPATH_TEST(scene_name, p, dir, base) \
    TEST(scene_name)                            \
    {                                           \
        char* t = NULL;                         \
                                                \
        printf("%s\n", p);                      \
                                                \
        t = strdup(p);                          \
        char* b = basename(t);                  \
        assert_string_equal(base, b);           \
        free(t);                                \
                                                \
        t = strdup(p);                          \
        char* d = dirname(t);                   \
        assert_string_equal(dir, d);            \
        free(t);                                \
    }

FILEPATH_TEST(root, "/", "/", "/")
FILEPATH_TEST(normal, "/usr/lib", "/usr", "lib")
FILEPATH_TEST(end_with_pathspr, "/usr/", "/", "usr")
FILEPATH_TEST(file_in_root, "/usr", "/", "usr")
FILEPATH_TEST(no_pathspr, "usr", ".", "usr")
FILEPATH_TEST(cur, ".", ".", ".")
FILEPATH_TEST(parent, "..", ".", "..")
FILEPATH_TEST(relative, "usr/tm.f", "usr", "tm.f")
FILEPATH_TEST(more_pathspr, "/usr///", "/", "usr")

int main(int argc, char* argv[])
{
    TEST_RUN();
    getchar();
    return 0;
}
