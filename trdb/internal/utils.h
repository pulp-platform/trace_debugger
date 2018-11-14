/*
 * trdb - Trace Debugger Software for the PULP platform
 *
 * Copyright (C) 2018 ETH Zurich and University of Bologna
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file utils.h
 * @author Robert Balas (balasr@student.ethz.ch)
 * @date 25 Aug 2018
 * @brief Utility and logging functions/macros
 */

#include <stdio.h>
#include <inttypes.h>
#include <syslog.h>
#include <assert.h>
#include "trace_debugger.h"

#ifdef NDEBUG
#    define TRDB_LEVEL 1
#    define TRDB_TRACE 0
#    define TRDB_VERBOSE_TESTS 0
#else
#    ifdef VERBOSE
#        define TRDB_LEVEL 3
#        define TRDB_TRACE 1
#        define TRDB_VERBOSE_TESTS 1
#    else
#        define TRDB_LEVEL 1
#        define TRDB_TRACE 0
#        define TRDB_VERBOSE_TESTS 0
#    endif
#endif

/* Different non-portable way to calculate packed bits requires __uint128_t
 * and little endian (or add the htole128 function)
 */
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__,
              "Machine must be little endian");

/* static inline void __attribute__((always_inline, format(printf, 2, 3))) */
/* trdb_log_null(struct trdb_ctx *ctx, const char *format, ...) */
/* { */
/* } */

void trdb_log_null(struct trdb_ctx *ctx, const char *format, ...);

#define trdb_log_cond(ctx, prio, arg...)                                       \
    do {                                                                       \
        if (trdb_get_log_priority(ctx) >= prio)                                \
            trdb_log(ctx, prio, __FILE__, __LINE__, __FUNCTION__, ##arg);      \
    } while (0)

#ifdef ENABLE_LOGGING
#    ifdef ENABLE_DEBUG
#        define dbg(ctx, arg...) trdb_log_cond(ctx, LOG_DEBUG, ##arg)
#    else
#        define dbg(ctx, arg...) trdb_log_null(ctx, ##arg)
#    endif
#    define info(ctx, arg...) trdb_log_cond(ctx, LOG_INFO, ##arg)
#    define err(ctx, arg...) trdb_log_cond(ctx, LOG_ERR, ##arg)
#else
#    define dbg(ctx, arg...) trdb_log_null(ctx, ##arg)
#    define info(ctx, arg...) trdb_log_null(ctx, ##arg)
#    define err(ctx, arg...) trdb_log_null(ctx, ##arg)
#endif

#ifdef HAVE_SECURE_GETENV
#    ifdef HAVE___SECURE_GETENV
#        define secure_getenv __secure_getenv
#    else
#        error neither secure_getenv nor __secure_getenv is available
#    endif
#else
#    define secure_getenv getenv
#endif

#define TRDB_EXPORT __attribute__((visibility("default")))

void trdb_log(struct trdb_ctx *ctx, int priority, const char *file, int line,
              const char *fn, const char *format, ...)
    __attribute__((format(printf, 6, 7)));

/**
 * Write formatted string to stderr with the filename, linenumber and function
 * prepended. Only used for tests.
 */
#define LOG_ERRT(format, ...)                                                  \
    do {                                                                       \
        if (TRDB_LEVEL > 0)                                                    \
            fprintf(stderr, "tests: %s:%d:0: %s(): " format, __FILE__,         \
                    __LINE__, __func__, ##__VA_ARGS__);                        \
    } while (false)
/* ## is gcc extension */

/**
 * Write formatted string to stderr with the filename, linenumber and function
 * prepended. Only used for tests.
 */
#define LOG_WARNT(format, ...)                                                 \
    do {                                                                       \
        if (TRDB_LEVEL > 1)                                                    \
            fprintf(stderr, "tests: %s:%d:0: %s(): " format, __FILE__,         \
                    __LINE__, __func__, ##__VA_ARGS__);                        \
    } while (false)

/**
 * Write formatted string to stdout with the filename, linenumber and function
 * prepended. Only used for tests.
 */
#define LOG_INFOT(format, ...)                                                 \
    do {                                                                       \
        if (TRDB_LEVEL > 2)                                                    \
            fprintf(stdout, "tests: %s:%d:0: %s(): " format, __FILE__,         \
                    __LINE__, __func__, ##__VA_ARGS__);                        \
    } while (false)

/**
 * Write formatted string to stdout with the filename, linenumber and function
 * prepended. Only used for tests.
 */
#define LOG_TRACET(format, ...)                                                \
    do {                                                                       \
        if (TRDB_TRACE)                                                        \
            fprintf(stdout, "tests: %s:%d:0: %s(): " format, __FILE__,         \
                    __LINE__, __func__, ##__VA_ARGS__);                        \
    } while (false)

/* TODO: make this typesafer */
/**
 * Return the size of an array in bytes.
 */
#define TRDB_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * Generate a mask with the first @p len bits set. @p len need to be smaller
 * than 64.
 */
#define MASK_FROM(len) ((1ull << len) - 1)
