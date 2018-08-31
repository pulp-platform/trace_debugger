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
 * @file util.h
 * @author Robert Balas (balasr@student.ethz.ch)
 * @date 25 Aug 2018
 * @brief Utility and logging functions/macros
 */

#include <stdio.h>
#include <inttypes.h>

#ifdef NDEBUG
#define TRDB_LEVEL 1
#define TRDB_TRACE 0
#define TRDB_TEST_DEBUG 0

#else

/* Sometimes we want quiet debug builds */
#ifdef QUIET
#define TRDB_LEVEL 1
#define TRDB_TRACE 0
#define TRDB_TEST_DEBUG 0
#else
#define TRDB_LEVEL 3
#define TRDB_TRACE 1
#define TRDB_TEST_DEBUG 1
#endif

#endif


/**
 * Write formatted string to stderr with the filename, linenumber and function
 * prepended.
 */
#define LOG_ERR(format, ...)                                                   \
    do {                                                                       \
        if (TRDB_LEVEL > 0)                                                    \
            fprintf(stderr, "%s:%d:0: %s(): " format, __FILE__, __LINE__,      \
                    __func__, ##__VA_ARGS__);                                  \
    } while (false)
/* ## is gcc extension */

/**
 * Write formatted string to stderr with the filename, linenumber and function
 * prepended.
 */
#define LOG_WARN(format, ...)                                                  \
    do {                                                                       \
        if (TRDB_LEVEL > 1)                                                    \
            fprintf(stderr, "%s:%d:0: %s(): " format, __FILE__, __LINE__,      \
                    __func__, ##__VA_ARGS__);                                  \
    } while (false)

/**
 * Write formatted string to stdout with the filename, linenumber and function
 * prepended.
 */
#define LOG_INFO(format, ...)                                                  \
    do {                                                                       \
        if (TRDB_LEVEL > 2)                                                    \
            fprintf(stdout, "%s:%d:0: %s(): " format, __FILE__, __LINE__,      \
                    __func__, ##__VA_ARGS__);                                  \
    } while (false)

/**
 * Write formatted string to stdout with the filename, linenumber and function
 * prepended.
 */
#define LOG_TRACE(format, ...)                                                 \
    do {                                                                       \
        if (TRDB_TRACE)                                                        \
            fprintf(stdout, "%s:%d:0: %s(): " format, __FILE__, __LINE__,      \
                    __func__, ##__VA_ARGS__);                                  \
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

/**
 * Load a file to a uint8_t array.
 *
 * The user is responsible to free the return pointer.
 *
 * @param fp the file to load
 * @param length written with the number of bytes of the file
 * @return pointer to the loaded file in memory
 */
uint8_t *file_to_char(FILE *fp, long *length);
