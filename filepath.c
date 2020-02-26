#include "filepath.h"

#include <stdlib.h>
#include <string.h>

static char filepath_const_cur[] = ".";
static char filepath_const_root[] = "/";

EXPORT_API char* dirname(char* path)
{
    char* p1 = strrchr(path, '/');
    char* p2 = strrchr(path, '\\');

    if ((NULL == p1) && (NULL == p2)) {
        return filepath_const_cur;
    }

    char* p = p1;
    if ((NULL != p1) && (NULL != p2)) {
        p = ((p1 > p2) ? p1 : p2);
    } else {
        p = ((NULL == p1) ? p2 : p1);
    }

    *p = '\0';

    if (path == p) {
        return filepath_const_root;
    }

    if ('\0' == p[1]) {
        if (p > path) {
            *p = '\0';
            return dirname(path);
        }
    }

    return path;
}

EXPORT_API char* basename(char* path)
{
    char* p1 = strrchr(path, '/');
    char* p2 = strrchr(path, '\\');

    if ((NULL == p1) && (NULL == p2)) {
        return path;
    }

    char* p = p1;
    if ((NULL != p1) && (NULL != p2)) {
        p = ((p1 > p2) ? p1 : p2);
    } else {
        p = ((NULL == p1) ? p2 : p1);
    }
    //
    //     if (p == path)
    //     {
    //         return filepath_const_root;
    //     }
    if ('\0' == p[1]) {
        if (p > path) {
            *p = '\0';
            return basename(path);
        }
        return p;
    }

    return p + 1;
}
