// SPDX-License-Identifier: AGPL-3.0-only

#pragma once

#include <util/attr.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef ssize_t res_t;

ATTR_PRINTF(5, 6)
ATTR_NORETURN
void die(res_t res, const char *filename, uint32_t line, const char *func, const char *fmt, ...);

#define ELIBC (res_t)(0 - errno)

#define DIE(res, ...) die((res), __FILE__, __LINE__, __func__, __VA_ARGS__);

#if TARGET == debug
#define ASSERT_RAW(expr, ...) \
    if (!(expr)) DIE(0, "assertion failed: " #expr __VA_ARGS__)
#else
#define ASSERT_RAW(expr, ...)
#endif

#define ASSERT_WITH(expr, ...) ASSERT_RAW((expr), ": " __VA_ARGS__)
#define ASSERT(expr) ASSERT_RAW((expr), "")
#define ASSERT_PTR(expr) ASSERT_RAW((expr), " is null")

#define TRY(res, ...)            \
    {                            \
        typeof((res)) r = (res); \
        if (r < 0) {             \
            __VA_ARGS__          \
            return r;            \
        }                        \
    }

#define TRY_BOOL(res, ...)       \
    {                            \
        typeof((res)) r = (res); \
        if (!r) {                \
            __VA_ARGS__          \
            return false;        \
        }                        \
    }

#define TRY_LIBC(res, ...)       \
    {                            \
        typeof((res)) r = (res); \
        if (r == -1) {           \
            int e = ELIBC;       \
            __VA_ARGS__          \
            return e;            \
        }                        \
    }

#define ERR(res) MAX(0, -res)
