#ifndef __filepath_H_
#define __filepath_H_

#include "exports.h"

#ifndef EXPORT_API
#ifdef __cplusplus
#define EXPORT_API extern "C"
#else
#define EXPORT_API extern
#endif
#endif  //    EXPORT_API

#ifdef _MSC_VER
EXPORT_API char* dirname(char* p);
EXPORT_API char* basename(char* p);
#else
#include <libgen.h>
#endif

#endif  //__filepath_H_
