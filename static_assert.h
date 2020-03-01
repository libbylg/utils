#ifndef _static_assert_H_
#define _static_assert_H_


#include <assert.h>


#ifndef STATIC_ASSERT
#ifdef __cplusplus
#define STATIC_ASSERT(const_expr...) static_assert(const_expr __VA_ARGS__)
#else
#define STATIC_ASSERT(const_expr...) static_assert(const_expr __VA_ARGS__)
#endif //
#endif //


#endif //_static_assert_H_
